#include "mini_app/demo/viz_test/demo_window.h"


namespace mini_app {

DemoWindow::DemoWindow() {

}

DemoWindow::~DemoWindow() {
  
}

void DemoWindow::Create(const gfx::Rect& bounds) {
  platform_window_ = CreatePlatformWindow(bounds);
  platform_window_->Show();
  if (widget_ != gfx::kNullAcceleratedWidget)
    Init();
}

void DemoWindow::Init() {

}

std::unique_ptr<ui::PlatformWindow> DemoWindow::CreatePlatformWindow(const gfx::Rect& bounds) {
  return nullptr;
}

void DemoWindow::OnBoundsChanged(const gfx::Rect& new_bounds) {

}

void DemoWindow::OnDamageRect(const gfx::Rect& damaged_region) {

}

void DemoWindow::DispatchEvent(ui::Event* event) {

}

void DemoWindow::OnCloseRequest() {
  
}

void DemoWindow::OnClosed() {
  
}

void DemoWindow::OnWindowStateChanged(ui::PlatformWindowState new_state) {

}

void DemoWindow::OnLostCapture() {

}

void DemoWindow::OnAcceleratedWidgetAvailable(gfx::AcceleratedWidget widget) {
  
}

void DemoWindow::OnAcceleratedWidgetDestroyed() {

}

void DemoWindow::OnActivationChanged(bool active) {

}

void DemoWindow::OnMouseEnter() {

}

} //namespace mini_app
