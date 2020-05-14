#include "base/logging.h"
#include "base/command_line.h"
#include "base/at_exit.h"
#include "base/message_loop/message_loop.h"
#include "base/run_loop.h"
#include "mini_app/frame_buffer/output_framebuffer.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>

#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/tools/ToolUtils.h"
#include "third_party/skia/include/core/SkEncodedImageFormat.h"

//打印fb驱动中fix结构信息，注：在fb驱动加载后，fix结构不可被修改。
void printFixedInfo(const fb_fix_screeninfo& finfo)
{
  LOG(INFO) << "============================";
  LOG(INFO) << "Fixed screen info: ";
  LOG(INFO) << "id: " << std::string(finfo.id, 16);
  LOG(INFO) << "smem_start: " << finfo.smem_start;
  LOG(INFO) << "smem_len: " << finfo.smem_start;
  LOG(INFO) << "type: " << finfo.type;
  LOG(INFO) << "type_aux: " << finfo.type_aux;
  LOG(INFO) << "visual: " << finfo.visual;
  LOG(INFO) << "xpanstep: " << finfo.xpanstep;
  LOG(INFO) << "ypanstep: " << finfo.ypanstep;
  LOG(INFO) << "ywrapstep: " << finfo.ywrapstep;
  LOG(INFO) << "line_length: " << finfo.line_length;
  LOG(INFO) << "mmio_start: " << finfo.mmio_start;
  LOG(INFO) << "mmio_len: " << finfo.mmio_len;
  LOG(INFO) << "accel: " << finfo.accel;
}


//打印fb驱动中var结构信息，注：fb驱动加载后，var结构可根据实际需要被重置
void printVariableInfo(const fb_var_screeninfo& vinfo)
{
  LOG(INFO) << "============================";
  LOG(INFO) << "Variable screen info:";
  LOG(INFO) << "xres: " << vinfo.xres;
  LOG(INFO) << "yres: " << vinfo.yres;
  LOG(INFO) << "xres_virtual: " << vinfo.xres_virtual;
  LOG(INFO) << "yres_virtual: " << vinfo.yres_virtual;
  LOG(INFO) << "xoffset: " << vinfo.xoffset;
  LOG(INFO) << "yoffset: " << vinfo.yoffset;
  LOG(INFO) << "bits_per_pixel: " << vinfo.bits_per_pixel;
  LOG(INFO) << "grayscale: " << vinfo.grayscale;
  LOG(INFO) << "red.offset: " << vinfo.red.offset;
  LOG(INFO) << "red.length: " << vinfo.red.length;
  LOG(INFO) << "red.msb_right: " << vinfo.red.msb_right;
  LOG(INFO) << "green.offset: " << vinfo.green.offset;
  LOG(INFO) << "green.length: " << vinfo.green.length;
  LOG(INFO) << "green.msb_right: " << vinfo.green.msb_right;
  LOG(INFO) << "blue.offset: " << vinfo.blue.offset;
  LOG(INFO) << "blue.length: " << vinfo.blue.length;
  LOG(INFO) << "blue.msb_right: " << vinfo.blue.msb_right;
  LOG(INFO) << "transp.offset: " << vinfo.transp.offset;
  LOG(INFO) << "transp.length: " << vinfo.transp.length;
  LOG(INFO) << "transp.msb_right: " << vinfo.transp.msb_right;
  LOG(INFO) << "nonstd: " << vinfo.nonstd;
  LOG(INFO) << "activate: " << vinfo.activate;
  LOG(INFO) << "height: " << vinfo.height;
  LOG(INFO) << "width: " << vinfo.width;
  LOG(INFO) << "accel_flags: " << vinfo.accel_flags;
  LOG(INFO) << "pixclock: " << vinfo.pixclock;
  LOG(INFO) << "left_margin: " << vinfo.left_margin;
  LOG(INFO) << "right_margin: " << vinfo.right_margin;
  LOG(INFO) << "upper_margin: " << vinfo.upper_margin;
  LOG(INFO) << "lower_margin: " << vinfo.lower_margin;
  LOG(INFO) << "hsync_len: " << vinfo.hsync_len;
  LOG(INFO) << "vsync_len: " << vinfo.vsync_len;
  LOG(INFO) << "sync: " << vinfo.sync;
  LOG(INFO) << "vmode: " << vinfo.vmode;
}

void* ReadFrameBuffer() {

  // const char *devfile = FRAMEBUFFER;
  // long int screensize = 0;
    void* membuffer = nullptr;
    int fbFd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    fbFd = open("/dev/graphics/fb0", O_RDWR, 0);
    if (fbFd < 0) {
        LOG(INFO) << "Error: cannot open framebuffer device";
        return nullptr;
    }

    //获取finfo信息并显示
    if (ioctl(fbFd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        LOG(INFO) << "Error reading fixed information";
        return nullptr;

    }
    else {
        printFixedInfo(finfo);
    }
    //获取vinfo信息并显示
    if (ioctl(fbFd, FBIOGET_VSCREENINFO, &vinfo) == -1){
        LOG(INFO) << "Error reading variable information";
        return nullptr;

    }
    else {
        printVariableInfo(vinfo);
    }

    vinfo.activate = 1;
    // vinfo.width = 1000;
    // vinfo.height = 1000;
    // vinfo.reserved[0] = 0;
    // vinfo.reserved[1] = 0;
    // vinfo.reserved[2] = 0;
    // vinfo.xoffset = 0;
    // vinfo.yoffset = 0;

    // vinfo.bits_per_pixel = 32;
    // vinfo.red.offset = 0;
    // vinfo.red.length = 8;
    // vinfo.green.offset = 8;
    // vinfo.green.length = 8;
    // vinfo.blue.offset = 16;
    // vinfo.blue.length = 8;
    // vinfo.transp.offset = 24;
    // vinfo.transp.length = 8;
    // vinfo.xres = vinfo.xres;
    // vinfo.xres_virtual = vinfo.xres;
    // vinfo.yres = vinfo.yres;
    // vinfo.yres_virtual = vinfo.yres;
    // vinfo.width = vinfo.xres;
    // vinfo.height = vinfo.yres;


    if(ioctl(fbFd, FBIOPUT_VSCREENINFO, &vinfo) == -1) {
        LOG(INFO) << "Error set variable information";
        return nullptr;
    }


    LOG(INFO) << "=======";
    membuffer = mmap(0, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fbFd, 0);
    if(membuffer == MAP_FAILED) {
        LOG(INFO) << "Error: Failed to map framebuffer device to memory";
        return membuffer;
    }

    // memset(membuffer, 0, finfo.smem_len);

    SkBitmap skBitmap;
    // skBitmap.setInfo(SkImageInfo::MakeN32Premul(vinfo.xres, vinfo.yres));
    // skBitmap.allocPixels();
    bool ret = skBitmap.installPixels(SkImageInfo::MakeN32Premul(vinfo.xres, vinfo.yres),
                            membuffer, vinfo.xres * 4);
    
    LOG(INFO) << "skbitmap installpixels is " << ret;

    SkCanvas skCanvas(skBitmap);
    SkPaint paint;
    paint.setColor(SK_ColorBLUE);
    // paint.setAntiAlias(true);
    paint.setStrokeWidth(4);

    // skCanvas.drawCircle(SkPoint::Make(500, 500), float(20), paint);
    skCanvas.drawRect(SkRect::MakeLTRB(300, 300, 500, 500), paint);

    // skCanvas.flush();

    // ToolUtils::EncodeImageToFile("1.png", skBitmap, SkEncodedImageFormat::kPNG, 100);
    // skCanvas.drawPaint(paint);

    // memcpy(membuffer, skBitmap.getPixels(), finfo.smem_len);
    

    // sleep(10);

    munmap(membuffer, finfo.smem_len);
    close(fbFd);

    return membuffer;
}

int main(int argc, char** argv) {
    base::AtExitManager exit;
    base::CommandLine::Init(argc, argv);
    base::MessageLoop message_loop;

    LOG(INFO) << "start mini_app ...";
    auto* fd = ReadFrameBuffer();
    if(fd == nullptr) {
        return -1;
    }
    base::RunLoop().Run();

    return 0;
}