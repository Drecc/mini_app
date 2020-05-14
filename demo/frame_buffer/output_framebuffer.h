#include <memory>
#include <linux/fb.h>

#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/skia/include/core/SkEncodedImageFormat.h"
#include "third_party/skia/tools/ToolUtils.h"


namespace mini_app {

class Output {
    virtual SkCanvas * Canvas() = 0;
    virtual std::string ToString() = 0;
};

class OutputFrameBuffer : public Output {
public:
    OutputFrameBuffer();
    virtual ~OutputFrameBuffer();

    //Output
    SkCanvas * Canvas() override;
    std::string ToString() override;

    bool Init(int width, int height);

private:
    void* frame_buffer_ = nullptr;
    void* frame_buffer_1_ = nullptr;
    void* frame_buffer_2_ = nullptr;
    int frame_buffer_fd_ = -1;
    int width_ = 0;
    int height_ = 0;
    int screen_size_ = 0;   //width * height * 4
    //framebuffer的相关信息
    fb_fix_screeninfo finfo_;
    fb_var_screeninfo vinfo_;

};






}   // namespace mini_app