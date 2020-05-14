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

namespace mini_app {


OutputFrameBuffer::OutputFrameBuffer() {
    
}

OutputFrameBuffer::~OutputFrameBuffer() {

}

SkCanvas * OutputFrameBuffer::Canvas() {
    return nullptr;
}





}   //namespace mini_app