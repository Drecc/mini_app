#include "mini_app/demo/frame_buffer/devices_info.h"

#include "ui/display/screen.h"

namespace mini_app {

DevicesInfo::DevicesInfo() {
    Init();
}

DevicesInfo::~DevicesInfo() {

}

gfx::Size DevicesInfo::GetScreenSize() {
    return screen_size_;
}

float DevicesInfo::GetScale() {
    return scale_;
}

void DevicesInfo::Init() {
    auto* screen = display::Screen::GetScreen();
    if(screen == nullptr) {
        screen_size_ =  gfx::Size();
        scale_ = 1.0;
        return;
    }
    auto display = screen->GetPrimaryDisplay();
    LOG(INFO) << display.ToString();
    screen_size_ =  display.size();
    scale_ = display.device_scale_factor();
}


}   //ink
