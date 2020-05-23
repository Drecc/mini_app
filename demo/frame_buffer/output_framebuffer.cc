#include "mini_app/demo/frame_buffer/output_framebuffer.h"

#include "base/logging.h"
#include "base/strings/stringprintf.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/kd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <errno.h>

namespace mini_app {

const char* const kFBDevice = "/dev/graphics/fb3";
OutputFrameBuffer* OutputFrameBuffer::g_instance_ = nullptr;

OutputFrameBuffer* OutputFrameBuffer::GetInstance() {
    //考虑后续加锁，由于不同的v8 context都有可能获取这个实例
    if(g_instance_ == nullptr) {
        g_instance_ = new OutputFrameBuffer();
    }
    return g_instance_;
}

OutputFrameBuffer::OutputFrameBuffer() {
    device_info_.reset(new DevicesInfo());
    screen_scale_width_ = device_info_->GetScreenSize().width() * device_info_->GetScale();
    screen_scale_height_= device_info_->GetScreenSize().height() * device_info_->GetScale();
}

OutputFrameBuffer::~OutputFrameBuffer() {  
    CloseFrameBuffer();
}

float OutputFrameBuffer::GetScale() {
    if(device_info_) {
        return device_info_->GetScale();
    }
    return 1.0;
}

void OutputFrameBuffer::SetDefalutVarInfo() {
    var_info_.reserved[0] = 0;
    var_info_.reserved[1] = 0;
    var_info_.reserved[2] = 0;
    var_info_.xoffset = 0;
    var_info_.yoffset = 0;
    var_info_.activate = FB_ACTIVATE_FORCE;

    var_info_.bits_per_pixel = 32;
    var_info_.red.offset = 0;
    var_info_.red.length = 8;
    var_info_.green.offset = 8;
    var_info_.green.length = 8;
    var_info_.blue.offset = 16;
    var_info_.blue.length = 8;
    var_info_.transp.offset = 24;
    var_info_.transp.length = 8;

    var_info_.xres = screen_scale_width_;
    var_info_.yres = screen_scale_height_;
    var_info_.xres_virtual = screen_scale_width_;
    var_info_.yres_virtual = screen_scale_height_;
    var_info_.width = screen_scale_width_;
    var_info_.height = screen_scale_height_;
}

bool OutputFrameBuffer::OpenFrameBuffer() {
    LOG(INFO) << "OutputFrameBuffer::OpenFrameBuffer";
    if(IsOpen()) {
        return true;
    }
    
    frame_buffer_fd_ = open(kFBDevice, O_RDWR);
    LOG(INFO) << "open fd: " << frame_buffer_fd_;
    if (frame_buffer_fd_ <= 0) {
        LOG(ERROR) << "open fd error " << errno;
        return false;
    }

    if (ioctl(frame_buffer_fd_, FBIOGET_VSCREENINFO, &var_info_) < 0) {
        LOG(ERROR) << "ioctl: get cursor var_info_ failed!";
        return false;
    }

    if (ioctl(frame_buffer_fd_, FBIOGET_FSCREENINFO, &fix_info_) < 0) {
        LOG(ERROR) << "ioctl: get fix screenInfo failed!";
        return false;
    }

    SetDefalutVarInfo();

    if (ioctl(frame_buffer_fd_, FBIOPUT_VSCREENINFO, &var_info_) < 0) {
        LOG(ERROR) << "ioctl: set cursor variable screeninfo failed!";
        return false;
    }

    mapped_mem_size_ = fix_info_.smem_len;
    mapped_mem_buffer_ =
        static_cast<uint32_t*>(mmap(0, mapped_mem_size_, PROT_READ | PROT_WRITE,
                                    MAP_SHARED, frame_buffer_fd_, 0));
    if (mapped_mem_buffer_ == nullptr) {
        LOG(ERROR) << "mmap failed!";
        return false;
    }

    bitmap_.reset(new SkBitmap());
    bitmap_->installPixels(
        SkImageInfo::MakeN32Premul(var_info_.xres, var_info_.yres), mapped_mem_buffer_,
        fix_info_.line_length);
    canvas_.reset(new SkCanvas(*bitmap_.get()));
    LOG(INFO) << "open success.";
    return true;
}

bool OutputFrameBuffer::IsOpen() {
    return frame_buffer_fd_ > 0;
}

void OutputFrameBuffer::Clear() {
  if (mapped_mem_buffer_ == nullptr) {
    return;
  }
  auto width = screen_scale_width_;
  auto height = screen_scale_height_;
  auto bitCount = 4;
  auto size = static_cast<size_t>(width * bitCount);
  for (auto y = 0; y < height; ++y) {
    memset(mapped_mem_buffer_ + width * y, 0, size);
  }
}

void OutputFrameBuffer::CloseFrameBuffer() {
  LOG(INFO) << "OutputFrameBuffer::CloseFrameBuffer";
  if(canvas_) {
      canvas_.reset();
  }
  if(bitmap_) {
      bitmap_.reset();
  }

  if (mapped_mem_buffer_ != NULL) {
    munmap(mapped_mem_buffer_, mapped_mem_size_);
    mapped_mem_buffer_ = nullptr;
  }

  if (frame_buffer_fd_ > 0) {
    close(frame_buffer_fd_);
    frame_buffer_fd_ = 0;
  }
}

SkCanvas* OutputFrameBuffer::Canvas() {
    if(canvas_) 
        return canvas_.get();
    return nullptr;
}

void OutputFrameBuffer::AddObserver(Observer* obs) {
    observer_list_.AddObserver(obs);
}

void OutputFrameBuffer::RemoveObserver(Observer* obs) {
    observer_list_.RemoveObserver(obs);
}

std::string OutputFrameBuffer::ToString() {
    if(!IsOpen()) {
        return std::string();
    }
    std::stringstream stream;
    stream << "============================" << "\n";
    stream << "Fixed screen info: " << "\n";
    stream << "id: " << fix_info_.id << "\n";
    stream << "smem_start: " << fix_info_.smem_start << "\n";
    stream << "smem_len: " << fix_info_.smem_start << "\n";
    stream << "type: " << fix_info_.type << "\n";
    stream << "type_aux: " << fix_info_.type_aux << "\n";
    stream << "visual: " << fix_info_.visual << "\n";
    stream << "xpanstep: " << fix_info_.xpanstep << "\n";
    stream << "ypanstep: " << fix_info_.ypanstep << "\n";
    stream << "ywrapstep: " << fix_info_.ywrapstep << "\n";
    stream << "line_length: " << fix_info_.line_length << "\n";
    stream << "mmio_start: " << fix_info_.mmio_start << "\n";
    stream << "mmio_len: " << fix_info_.mmio_len << "\n";
    stream << "accel: " << fix_info_.accel << "\n";

    stream << "============================" << "\n";
    stream << "Variable screen info:" << "\n";
    stream << "xres: " << var_info_.xres << "\n";
    stream << "yres: " << var_info_.yres << "\n";
    stream << "xres_virtual: " << var_info_.xres_virtual << "\n";
    stream << "yres_virtual: " << var_info_.yres_virtual << "\n";
    stream << "xoffset: " << var_info_.xoffset << "\n";
    stream << "yoffset: " << var_info_.yoffset << "\n";
    stream << "bits_per_pixel: " << var_info_.bits_per_pixel << "\n";
    stream << "grayscale: " << var_info_.grayscale << "\n";
    stream << "red.offset: " << var_info_.red.offset << "\n";
    stream << "red.length: " << var_info_.red.length << "\n";
    stream << "red.msb_right: " << var_info_.red.msb_right << "\n";
    stream << "green.offset: " << var_info_.green.offset << "\n";
    stream << "green.length: " << var_info_.green.length << "\n";
    stream << "green.msb_right: " << var_info_.green.msb_right << "\n";
    stream << "blue.offset: " << var_info_.blue.offset << "\n";
    stream << "blue.length: " << var_info_.blue.length << "\n";
    stream << "blue.msb_right: " << var_info_.blue.msb_right << "\n";
    stream << "transp.offset: " << var_info_.transp.offset << "\n";
    stream << "transp.length: " << var_info_.transp.length << "\n";
    stream << "transp.msb_right: " << var_info_.transp.msb_right << "\n";
    stream << "nonstd: " << var_info_.nonstd << "\n";
    stream << "activate: " << var_info_.activate << "\n";
    stream << "height: " << var_info_.height << "\n";
    stream << "width: " << var_info_.width << "\n";
    stream << "accel_flags: " << var_info_.accel_flags << "\n";
    stream << "pixclock: " << var_info_.pixclock << "\n";
    stream << "left_margin: " << var_info_.left_margin << "\n";
    stream << "right_margin: " << var_info_.right_margin << "\n";
    stream << "upper_margin: " << var_info_.upper_margin << "\n";
    stream << "lower_margin: " << var_info_.lower_margin << "\n";
    stream << "hsync_len: " << var_info_.hsync_len << "\n";
    stream << "vsync_len: " << var_info_.vsync_len << "\n";
    stream << "sync: " << var_info_.sync << "\n";
    stream << "vmode: " << var_info_.vmode << "\n";

    return stream.str();
}
}   //namespace mini_app