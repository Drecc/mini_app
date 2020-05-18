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

const char* FRAME_BUFFER_FILE = "/dev/graphics/fb0";

namespace mini_app {


OutputFrameBuffer::OutputFrameBuffer() {
    
}

OutputFrameBuffer::~OutputFrameBuffer() {
    if(frame_buffer_ != nullptr) {
        munmap(frame_buffer_, finfo_.smem_len);
    }
    if(frame_buffer_fd_ > 0) {
        close(frame_buffer_fd_);
    }
}

SkCanvas * OutputFrameBuffer::Canvas() {
    return nullptr;
}

bool OutputFrameBuffer::Init(int width, int height) {
    if(frame_buffer_ != nullptr)
        return true;
    frame_buffer_fd_ = open(FRAME_BUFFER_FILE, O_RDWR, 0);
    if(frame_buffer_fd_ < 0) {
        LOG(ERROR) << "open framebuffer fail";
        return false;
    }
    if(ioctl(frame_buffer_fd_, FBIOGET_FSCREENINFO, &finfo_) < 0) {
        LOG(ERROR) << "get fscreeninfo information fail";
        return false;
    }
    if (ioctl(frame_buffer_fd_, FBIOGET_VSCREENINFO, &vinfo_) < 0){
        LOG(ERROR) << "get vscreeninfo information fail";
        return false;

    }
    width_ = width;
    height_ = height;

    // vinfo_.xoffset = 0;
    // vinfo_.yoffset = 0;
    // vinfo_.bits_per_pixel = 32;
    // vinfo_.activate = FB_ACTIVATE_FORCE;
    // vinfo_.reserved[0] = 0;
    // vinfo_.reserved[1] = 0;
    // vinfo_.reserved[2] = 0;

    // vinfo_.bits_per_pixel = 32;
    // vinfo_.red.offset = 0;
    // vinfo_.red.length = 8;
    // vinfo_.green.offset = 8;
    // vinfo_.green.length = 8;
    // vinfo_.blue.offset = 16;
    // vinfo_.blue.length = 8;
    // vinfo_.transp.offset = 24;
    // vinfo_.transp.length = 8;

    // vinfo_.xres = 1200;
    // vinfo_.xres_virtual = 1200;
    // vinfo_.yres = 1920;
    // vinfo_.yres_virtual = 1920;
    // vinfo_.width = 1200;
    // vinfo_.height = 1920;



    // if (ioctl(frame_buffer_fd_, FBIOPUT_VSCREENINFO, &vinfo_) < 0) {
    //     LOG(ERROR) << "Unable to set variable screeninfo";
    //     return false;
    // }

    if (ioctl(frame_buffer_fd_, FBIOGET_VSCREENINFO, &vinfo_) < 0){
        LOG(ERROR) << "get vscreeninfo information fail";
        return false;

    }
    screen_size_ = vinfo_.xres * vinfo_.yres * vinfo_.bits_per_pixel / 8;

    frame_buffer_ = mmap(0, screen_size_, PROT_READ | PROT_WRITE, MAP_SHARED, frame_buffer_fd_, 0);
    if (frame_buffer_ == MAP_FAILED) {
        LOG(ERROR) << "failed to mmap framebuffer";
        return false;
    }



    LOG(INFO) << ToString();

    memset(frame_buffer_, 0, screen_size_);


    int x = 100;
    int y = 100;
    int location = x * (vinfo_.bits_per_pixel / 8) + y  *  finfo_.line_length;


    *((char*)frame_buffer_ + location) = 100;  /* 蓝色的色深 */  /*直接赋值来改变屏幕上某点的颜色*/
    *((char*)frame_buffer_ + location + 1) = 15; /* 绿色的色深*/   
    *((char*)frame_buffer_ + location + 2) = 200; /* 红色的色深*/   
    *((char*)frame_buffer_ + location + 3) = 0;  /* 是否透明*/ 

    // SkPaint paint;
    // paint.setColor(SK_ColorRED);
    // paint.setStrokeWidth(5);

    // auto* skBitmap = new SkBitmap();
    // skBitmap->installPixels(SkImageInfo::MakeN32Premul(vinfo_.xres, vinfo_.yres),
    //                         frame_buffer_, vinfo_.xres * 4);
    // auto* skCanvas = new SkCanvas(*skBitmap);
    // // skCanvas->scale(380, 380);
    // skCanvas->drawCircle(200, 200, 20, paint);
    // skCanvas->flush();

    return true;
}

std::string OutputFrameBuffer::ToString() {
    if(frame_buffer_fd_ < 0) {
        return std::string();
    }
    std::stringstream stream;
    stream << "============================" << "\n";
    stream << "Fixed screen info: " << "\n";
    stream << "id: " << finfo_.id << "\n";
    stream << "smem_start: " << finfo_.smem_start << "\n";
    stream << "smem_len: " << finfo_.smem_start << "\n";
    stream << "type: " << finfo_.type << "\n";
    stream << "type_aux: " << finfo_.type_aux << "\n";
    stream << "visual: " << finfo_.visual << "\n";
    stream << "xpanstep: " << finfo_.xpanstep << "\n";
    stream << "ypanstep: " << finfo_.ypanstep << "\n";
    stream << "ywrapstep: " << finfo_.ywrapstep << "\n";
    stream << "line_length: " << finfo_.line_length << "\n";
    stream << "mmio_start: " << finfo_.mmio_start << "\n";
    stream << "mmio_len: " << finfo_.mmio_len << "\n";
    stream << "accel: " << finfo_.accel << "\n";

    stream << "============================" << "\n";
    stream << "Variable screen info:" << "\n";
    stream << "xres: " << vinfo_.xres << "\n";
    stream << "yres: " << vinfo_.yres << "\n";
    stream << "xres_virtual: " << vinfo_.xres_virtual << "\n";
    stream << "yres_virtual: " << vinfo_.yres_virtual << "\n";
    stream << "xoffset: " << vinfo_.xoffset << "\n";
    stream << "yoffset: " << vinfo_.yoffset << "\n";
    stream << "bits_per_pixel: " << vinfo_.bits_per_pixel << "\n";
    stream << "grayscale: " << vinfo_.grayscale << "\n";
    stream << "red.offset: " << vinfo_.red.offset << "\n";
    stream << "red.length: " << vinfo_.red.length << "\n";
    stream << "red.msb_right: " << vinfo_.red.msb_right << "\n";
    stream << "green.offset: " << vinfo_.green.offset << "\n";
    stream << "green.length: " << vinfo_.green.length << "\n";
    stream << "green.msb_right: " << vinfo_.green.msb_right << "\n";
    stream << "blue.offset: " << vinfo_.blue.offset << "\n";
    stream << "blue.length: " << vinfo_.blue.length << "\n";
    stream << "blue.msb_right: " << vinfo_.blue.msb_right << "\n";
    stream << "transp.offset: " << vinfo_.transp.offset << "\n";
    stream << "transp.length: " << vinfo_.transp.length << "\n";
    stream << "transp.msb_right: " << vinfo_.transp.msb_right << "\n";
    stream << "nonstd: " << vinfo_.nonstd << "\n";
    stream << "activate: " << vinfo_.activate << "\n";
    stream << "height: " << vinfo_.height << "\n";
    stream << "width: " << vinfo_.width << "\n";
    stream << "accel_flags: " << vinfo_.accel_flags << "\n";
    stream << "pixclock: " << vinfo_.pixclock << "\n";
    stream << "left_margin: " << vinfo_.left_margin << "\n";
    stream << "right_margin: " << vinfo_.right_margin << "\n";
    stream << "upper_margin: " << vinfo_.upper_margin << "\n";
    stream << "lower_margin: " << vinfo_.lower_margin << "\n";
    stream << "hsync_len: " << vinfo_.hsync_len << "\n";
    stream << "vsync_len: " << vinfo_.vsync_len << "\n";
    stream << "sync: " << vinfo_.sync << "\n";
    stream << "vmode: " << vinfo_.vmode << "\n";

    return stream.str();
}

}   //namespace mini_app