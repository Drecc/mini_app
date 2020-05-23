#ifndef MINI_APP_DEMO_FRAME_BUFFER_OUTPUT_FRAMEBUFFER_H_
#define MINI_APP_DEMO_FRAME_BUFFER_OUTPUT_FRAMEBUFFER_H_

#include <memory>
#include <linux/fb.h>

#include "base/observer_list.h"
#include "mini_app/demo/frame_buffer/devices_info.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkCanvas.h"

namespace mini_app {

class Output {
    virtual SkCanvas * Canvas() = 0;
    virtual std::string ToString() = 0;
};

class OutputFrameBuffer : public Output {
public:
    class Observer : public base::CheckedObserver {
    public:
        virtual void FrameBufferWillClose() {}
        virtual void FrameBufferWillOpen() {}
    };

    static OutputFrameBuffer* GetInstance();

    void AddObserver(Observer* obs);
    void RemoveObserver(Observer* obs);

    //Output
    SkCanvas* Canvas() override;
    std::string ToString() override;

    float GetScale();
    void Clear();

    bool IsOpen();
    bool OpenFrameBuffer();
    void CloseFrameBuffer();

private:
    explicit OutputFrameBuffer();
    OutputFrameBuffer(const OutputFrameBuffer&) = delete;
    OutputFrameBuffer(OutputFrameBuffer&&) = delete;
    ~OutputFrameBuffer();
    void SetDefalutVarInfo();

    static OutputFrameBuffer* g_instance_;
    int frame_buffer_fd_ = 0;
    size_t mapped_mem_size_ = 0;
    uint32_t* mapped_mem_buffer_ = nullptr;
    fb_fix_screeninfo fix_info_;
    fb_var_screeninfo var_info_;
    int screen_scale_width_;
    int screen_scale_height_;
    std::unique_ptr<DevicesInfo> device_info_;
    std::unique_ptr<SkBitmap> bitmap_;
    std::unique_ptr<SkCanvas> canvas_;

    base::ObserverList<Observer> observer_list_;

};

}   // namespace mini_app

#endif //MINI_APP_DEMO_FRAME_BUFFER_OUTPUT_FRAMEBUFFER_H_
