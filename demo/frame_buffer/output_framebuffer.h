#include <memory>

#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/skia/include/core/SkEncodedImageFormat.h"
#include "third_party/skia/tools/ToolUtils.h"


namespace mini_app {

class Output {
    virtual SkCanvas * Canvas() = 0;
};

class OutputFrameBuffer : public Output {
public:
    OutputFrameBuffer();
    virtual ~OutputFrameBuffer();

    SkCanvas * Canvas() override;

private:
    void* frameBuffer_;

};






}   // namespace mini_app