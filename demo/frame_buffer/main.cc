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
#include <iostream>
 
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/skia/include/core/SkPaint.h"
#include "third_party/skia/include/core/SkRect.h"
#include "third_party/skia/include/core/SkStream.h"
#include "third_party/skia/include/core/SkMatrix.h"
#include "third_party/skia/include/core/SkImageInfo.h"

#include "third_party/skia/include/codec/SkCodec.h"
#include "third_party/skia/tools/ToolUtils.h"

#define FRAMEBUFFER "/dev/graphics/fb0"
#define OUT_DIR "/sdcard/"
// #endif

#define ERR_EXIT(m) \
do{\
    perror(m);\
    exit(EXIT_FAILURE);\
}while(0) \

 
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
unsigned int *frameBuffer = 0;
#define SKIA_SCALE 0
//打印fb驱动中fix结构信息，注：在fb驱动加载后，fix结构不可被修改。
void printFixedInfo()
{
    printf("Fixed screen info:\n"
            "\tid: %s\n"
            "\tsmem_start: 0x%lx\n"
            "\tsmem_len: %d\n"
            "\ttype: %d\n"
            "\ttype_aux: %d\n"
            "\tvisual: %d\n"
            "\txpanstep: %d\n"
            "\typanstep: %d\n"
            "\tywrapstep: %d\n"
            "\tline_length: %d\n"
            "\tmmio_start: 0x%lx\n"
            "\tmmio_len: %d\n"
            "\taccel: %d\n"
            "\n", finfo.id, finfo.smem_start, finfo.smem_len, finfo.type,
            finfo.type_aux, finfo.visual, finfo.xpanstep, finfo.ypanstep,
            finfo.ywrapstep, finfo.line_length, finfo.mmio_start,
            finfo.mmio_len, finfo.accel);
}
 
//打印fb驱动中var结构信息，注：fb驱动加载后，var结构可根据实际需要被重置
void printVariableInfo()
{
    printf("Variable screen info:\n"
            "\txres: %d\n"
            "\tyres: %d\n"
            "\txres_virtual: %d\n"
            "\tyres_virtual: %d\n"
            "\tyoffset: %d\n"
            "\txoffset: %d\n"
            "\tbits_per_pixel: %d\n"
            "\tgrayscale: %d\n"
            "\tnonstd: %d\n"
            "\tactivate: %d\n"
            "\theight: %d\n"
            "\twidth: %d\n"
            "\taccel_flags: 0x%x\n"
            "\tpixclock: %d\n"
            "\tleft_margin: %d\n"
            "\tright_margin: %d\n"
            "\tupper_margin: %d\n"
            "\tlower_margin: %d\n"
            "\thsync_len: %d\n"
            "\tvsync_len: %d\n"
            "\tsync: %d\n"
            "\tvmode: %d\n"
            "\n", vinfo.xres, vinfo.yres, vinfo.xres_virtual,
            vinfo.yres_virtual, vinfo.xoffset, vinfo.yoffset,
            vinfo.bits_per_pixel, vinfo.grayscale, vinfo.nonstd,
            vinfo.activate, vinfo.height, vinfo.width, vinfo.accel_flags,
            vinfo.pixclock, vinfo.left_margin, vinfo.right_margin,
            vinfo.upper_margin, vinfo.lower_margin, vinfo.hsync_len,
            vinfo.vsync_len, vinfo.sync, vinfo.vmode);
}
 
//画大小为width*height的同色矩阵，8alpha+8reds+8greens+8blues
void drawRect_rgb32(int x0, int y0, int width, int height, int color)
{
    printf("x: %d, y:%d, width:%d,height:%d\n",x0,y0,width,height);
   const int bytesPerPixel = 4;
   //finfo.line_length 是一行的字节数
   const int stride = finfo.line_length / bytesPerPixel;  // 有多少个像素
   const unsigned int colorRGBA = color;
 
   //dest指向画图的起点
   unsigned int *dest = (unsigned int *) (frameBuffer)+ (y0 + vinfo.yoffset) * stride + (x0 +vinfo.xoffset);
 
   int x, y;
   for (y = 0; y < height; ++y)
    {
       for (x = 0; x < width; ++x)
       {
           dest[x] = colorRGBA;
       }
       dest += stride;
    }
}
 
//画大小为width*height的同色矩阵，5reds+6greens+5blues
void drawRect_rgb16(int x0, int y0, int width, int height, int color)
{
    const int bytesPerPixel = 2;
    const int stride = finfo.line_length / bytesPerPixel;
    const int red = (color & 0xff0000) >> (16 + 3);
    const int green = (color & 0xff00) >> (8 + 2);
    const int blue = (color & 0xff) >> 3;
    const short color16 = blue | (green << 5) | (red << (5 + 6));
 
    short *dest = (short *) (frameBuffer) + (y0 + vinfo.yoffset) * stride
            + (x0 + vinfo.xoffset);
 
    int x, y;
    for (y = 0; y < height; ++y)
    {
        for (x = 0; x < width; ++x)
        {
            dest[x] = color16;
        }
        dest += stride;
    }
}
 
//画大小为width*height的同色矩阵，5reds+5greens+5blues
void drawRect_rgb15(int x0, int y0, int width, int height, int color)
{
    const int bytesPerPixel = 2;
    const int stride = finfo.line_length / bytesPerPixel;
    const int red = (color & 0xff0000) >> (16 + 3);
    const int green = (color & 0xff00) >> (8 + 3);
    const int blue = (color & 0xff) >> 3;
    const short color15 = blue | (green << 5) | (red << (5 + 5)) | 0x8000;
 
    short *dest = (short *) (frameBuffer) + (y0 + vinfo.yoffset) * stride
            + (x0 + vinfo.xoffset);
 
    int x, y;
    for (y = 0; y < height; ++y)
    {
        for (x = 0; x < width; ++x)
        {
            dest[x] = color15;
        }
        dest += stride;
    }
}
 
void drawRect(int x0, int y0, int width, int height, int color)
{
    switch (vinfo.bits_per_pixel)
    {
    case 32:
        drawRect_rgb32(x0, y0, width, height, color);
        break;
    case 16:
        drawRect_rgb16(x0, y0, width, height, color);
        break;
    case 15:
        drawRect_rgb15(x0, y0, width, height, color);
        break;
    default:
        printf("Warning: drawRect() not implemented for color depth %i\n",
                vinfo.bits_per_pixel);
        break;
    }
}
 
int main(int argc, char **argv)
{
    const char *devfile = FRAMEBUFFER;
    long int screensize = 0;
    int fbFd = 0;
 
    fbFd = open(devfile, O_RDWR);
    if (fbFd == -1)
    {
        ERR_EXIT("Error: cannot open framebuffer device");
    }

    //获取vinfo信息并显示
    if (ioctl(fbFd, FBIOGET_VSCREENINFO, &vinfo) == -1)
    {
        ERR_EXIT("Error reading variable information");
    }

    vinfo.reserved[0] = 0;
    vinfo.reserved[1] = 0;
    vinfo.reserved[2] = 0;
    vinfo.xoffset = 0;
    vinfo.yoffset = 0;

    vinfo.bits_per_pixel = 32;
    vinfo.red.offset = 0;
    vinfo.red.length = 8;
    vinfo.green.offset = 8;
    vinfo.green.length = 8;
    vinfo.blue.offset = 16;
    vinfo.blue.length = 8;
    vinfo.transp.offset = 24;
    vinfo.transp.length = 8;


    vinfo.activate = FB_ACTIVATE_FORCE;
    vinfo.xres = 1216;
    vinfo.xres_virtual = 1216;
    vinfo.yres = 1920;
    vinfo.yres_virtual = 1920;
    vinfo.width = 1216;
    vinfo.height = 1920;


   if (ioctl (fbFd, FBIOPUT_VSCREENINFO, &vinfo) == -1)
    {
       ERR_EXIT("Error set variable information");
    }

    //获取vinfo信息并显示
    if (ioctl(fbFd, FBIOGET_VSCREENINFO, &vinfo) == -1)
    {
        ERR_EXIT("Error reading variable information");
    }

    printVariableInfo();

 
    //获取finfo信息并显示
    if (ioctl(fbFd, FBIOGET_FSCREENINFO, &finfo) == -1)
    {
        ERR_EXIT("Error reading fixed information");
    }

    // 打印固定信息
    printFixedInfo();
    
    // screensize表示framebuffer 的大小
    screensize = finfo.smem_len;
 
    frameBuffer = (unsigned int*) mmap(0, screensize, PROT_READ | PROT_WRITE,
            MAP_SHARED, fbFd, 0);
    if (frameBuffer == MAP_FAILED )
    {
        ERR_EXIT("Error: Failed to map framebuffer device to memory");
    }
 
    memset(frameBuffer, 0, screensize);
 
    drawRect(vinfo.xres / 8, vinfo.yres / 8, vinfo.xres / 4, vinfo.yres / 4,
            0xff000088);
    // drawRect(vinfo.xres * 3 / 8, vinfo.yres * 3 / 8, vinfo.xres / 4,
    //         vinfo.yres / 4, 0xff00ff00);
    // drawRect(vinfo.xres * 5 / 8, vinfo.yres * 5 / 8, vinfo.xres / 4,
    //         vinfo.yres / 4, 0xff0000ff);
 
    // sleep(2);
    // printf(" Done to render rectangles.\n");
    // memset(frameBuffer, 0, screensize);

    SkPaint paint;
    paint.setColor(SK_ColorGREEN);
    paint.setStrokeWidth(1);

    auto* skBitmap = new SkBitmap();
    skBitmap->installPixels(SkImageInfo::MakeN32Premul(vinfo.xres, vinfo.yres),
                            frameBuffer, finfo.line_length);
    auto* skCanvas = new SkCanvas(*skBitmap);
    printf("%zu \n", skBitmap->computeByteSize());
    // skCanvas->scale(380, 380);
    // skCanvas->drawColor(SK_ColorBLUE);
    skCanvas->drawCircle(200, 200, 20, paint);
    // skCanvas->drawRoundRect(SkRect::MakeWH(300, 480), 150, 300, paint);
    // drawRect(vinfo.xres / 8, vinfo.yres / 8, vinfo.xres / 4, vinfo.yres / 4,
    //     0xff000088);
    skCanvas->flush();

    // SkBitmap bitmap;
    // bitmap.setInfo(SkImageInfo::Make(vinfo.xres, vinfo.yres, kRGBA_8888_SkColorType, kOpaque_SkAlphaType));
    // bitmap.allocPixels();
    // printf("%zu \n", bitmap.computeByteSize());
    // SkCanvas canvas(bitmap);
    // canvas.drawCircle(200, 200, 20, paint);
    // memcpy(frameBuffer, bitmap.getPixels(), bitmap.computeByteSize());


    sleep(2);
    getchar();
    printf(" End Loop.\n");
    munmap(frameBuffer, screensize);    //解除内存映射，与mmap对应

    close(fbFd);
    return 0;
}