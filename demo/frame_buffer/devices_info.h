#ifndef MINI_APP_DEMO_FRAME_BUFFER_DEVICES_INFO_H_
#define MINI_APP_DEMO_FRAME_BUFFER_DEVICES_INFO_H_

#include "ui/gfx/geometry/size.h"

namespace mini_app {

class DevicesInfo {
public:
    DevicesInfo();
    ~DevicesInfo();
    gfx::Size GetScreenSize();
    float GetScale();
private:
    void Init();
    gfx::Size screen_size_;
    float scale_;
};

}  // namespace mini_app

#endif //MINI_APP_DEMO_FRAME_BUFFER_DEVICES_INFO_H_
