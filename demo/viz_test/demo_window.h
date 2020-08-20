#ifndef MINI_APP_DEMO_VIZ_TEST_DEMO_WINDOW_H_
#define MINI_APP_DEMO_VIZ_TEST_DEMO_WINDOW_H_

#include "base/macros.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/platform_window/platform_window_delegate.h"
#include "ui/platform_window/platform_window_init_properties.h"
#include "ui/platform_window/platform_window.h"

namespace mini_app {

class DemoWindow : public ui::PlatformWindowDelegate {
public:
  DemoWindow();
  ~DemoWindow() override;

  void Create(const gfx::Rect& bounds);

protected:
  void OnBoundsChanged(const gfx::Rect& new_bounds) override;
  void OnDamageRect(const gfx::Rect& damaged_region) override;
  void DispatchEvent(ui::Event* event) override;
  void OnCloseRequest() override;
  void OnClosed() override;
  void OnWindowStateChanged(ui::PlatformWindowState new_state) override;
  void OnLostCapture() override;
  void OnAcceleratedWidgetAvailable(gfx::AcceleratedWidget widget) override;
  void OnAcceleratedWidgetDestroyed() override;
  void OnActivationChanged(bool active) override;
  void OnMouseEnter() override;

private:
  void Init();
  std::unique_ptr<ui::PlatformWindow> CreatePlatformWindow(const gfx::Rect& bounds);

  std::unique_ptr<ui::PlatformWindow> platform_window_;
  gfx::AcceleratedWidget widget_;

  DISALLOW_COPY_AND_ASSIGN(DemoWindow);
};

} //namespace mini_app

#endif //MINI_APP_DEMO_VIZ_TEST_DEMO_WINDOW_H_