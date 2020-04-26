#ifndef MINI_APP_CORE_UI_VIEWS_APPS_SHAPED_APP_WINDOW_TARGETER_H_
#define MINI_APP_CORE_UI_VIEWS_APPS_SHAPED_APP_WINDOW_TARGETER_H_

#include "base/macros.h"
#include "ui/aura/window_targeter.h"

namespace mini_app {

namespace UI {

class ChromeNativeAppWindowViews;

class ShapedAppWindowTargeter : public aura::WindowTargeter {
 public:
  explicit ShapedAppWindowTargeter(ChromeNativeAppWindowViews* app_window);
  ~ShapedAppWindowTargeter() override;

 private:
  // aura::WindowTargeter:
  std::unique_ptr<aura::WindowTargeter::HitTestRects> GetExtraHitTestShapeRects(
      aura::Window* target) const override;

  ChromeNativeAppWindowViews* app_window_;

  DISALLOW_COPY_AND_ASSIGN(ShapedAppWindowTargeter);
};

}  // namespace UI

}   // namespace mini_app

#endif  // MINI_APP_CORE_UI_VIEWS_APPS_SHAPED_APP_WINDOW_TARGETER_H_
