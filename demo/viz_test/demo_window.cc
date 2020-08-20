#include "mini_app/demo/viz_test/demo_window.h"

#include "base/logging.h"

#if defined(USE_OZONE)
#include "ui/ozone/public/ozone_platform.h"
#endif

#if defined(OS_WIN)
#include "ui/base/cursor/cursor_loader_win.h"
#include "ui/platform_window/win/win_window.h"
#endif

#if defined(USE_X11)
#include "ui/platform_window/x11/x11_window.h"
#endif
namespace mini_app {

DemoWindow::DemoWindow() {

}

DemoWindow::~DemoWindow() {

}

void DemoWindow::Create(const gfx::Rect& bounds) {
  platform_window_ = CreatePlatformWindow(bounds);
  platform_window_->Show();
}

void DemoWindow::Init() {

}

std::unique_ptr<ui::PlatformWindow> DemoWindow::CreatePlatformWindow(const gfx::Rect& bounds) {
  ui::PlatformWindowInitProperties property;
  property.bounds = bounds;
#if defined(USE_OZONE)
    return ui::OzonePlatform::GetInstance()->CreatePlatformWindow(
        this, std::move(property));
#elif defined(OS_WIN)
    return std::make_unique<ui::WinWindow>(this, property.bounds);
#elif defined(USE_X11)
    auto x11_window = std::make_unique<ui::X11Window>(this);
    x11_window->Initialize(std::move(property));
    return x11_window;
#else
    NOTIMPLEMENTED();
    return nullptr;
#endif
}

void DemoWindow::OnBoundsChanged(const gfx::Rect& new_bounds) {
  LOG(INFO) << "OnBoundsChanged " << new_bounds.ToString();
}

void DemoWindow::OnDamageRect(const gfx::Rect& damaged_region) {
  LOG(INFO) << "OnDamageRect " << damaged_region.ToString();
}

void DemoWindow::DispatchEvent(ui::Event* event) {
  LOG(INFO) << "DispatchEvent";
}

void DemoWindow::OnCloseRequest() {
  LOG(INFO) << "OnCloseRequest";
}

void DemoWindow::OnClosed() {
}

void DemoWindow::OnWindowStateChanged(ui::PlatformWindowState new_state) {
}

void DemoWindow::OnLostCapture() {
}

void DemoWindow::OnAcceleratedWidgetAvailable(gfx::AcceleratedWidget widget) {
  LOG(INFO) << "OnAcceleratedWidgetAvailable";
  //get window handle
  widget_ = widget;
}

void DemoWindow::OnAcceleratedWidgetDestroyed() {
}

void DemoWindow::OnActivationChanged(bool active) {
}

void DemoWindow::OnMouseEnter() {
}

} //namespace mini_app
