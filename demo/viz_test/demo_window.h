#ifndef MINI_APP_DEMO_VIZ_TEST_DEMO_WINDOW_H_
#define MINI_APP_DEMO_VIZ_TEST_DEMO_WINDOW_H_

#include "base/macros.h"
#include "ui/platform_window/platform_window_delegate.h"
#include "ui/platform_window/platform_window_init_properties.h"
#include "ui/platform_window/platform_window.h"

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