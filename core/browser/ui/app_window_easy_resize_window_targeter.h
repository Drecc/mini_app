#ifndef MINI_APP_CORE_UI_VIEWS_APPS_APP_WINDOW_EASY_RESIZE_WINDOW_TARGETER_H_
#define MINI_APP_CORE_UI_VIEWS_APPS_APP_WINDOW_EASY_RESIZE_WINDOW_TARGETER_H_

#include "base/macros.h"
#include "ui/wm/core/easy_resize_window_targeter.h"

namespace ui {
class BaseWindow;
}

namespace mini_app {

namespace UI {

// An EasyResizeEventTargeter whose behavior depends on the state of the app
// window.
class AppWindowEasyResizeWindowTargeter : public wm::EasyResizeWindowTargeter {
 public:
  AppWindowEasyResizeWindowTargeter(const gfx::Insets& insets,
                                    ui::BaseWindow* native_app_window);

  ~AppWindowEasyResizeWindowTargeter() override;

 protected:
  // aura::WindowTargeter:
  bool GetHitTestRects(aura::Window* window,
                       gfx::Rect* rect_mouse,
                       gfx::Rect* rect_touch) const override;

 private:
  ui::BaseWindow* native_app_window_;

  DISALLOW_COPY_AND_ASSIGN(AppWindowEasyResizeWindowTargeter);
};

}  // namespace UI

}   // namespace mini_app

#endif  // MINI_APP_CORE_UI_VIEWS_APPS_APP_WINDOW_EASY_RESIZE_WINDOW_TARGETER_H_
