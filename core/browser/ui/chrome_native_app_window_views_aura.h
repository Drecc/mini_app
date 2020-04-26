#ifndef MINI_APP_CORE_UI_VIEWS_APPS_CHROME_NATIVE_APP_WINDOW_VIEWS_AURA_H_
#define MINI_APP_CORE_UI_VIEWS_APPS_CHROME_NATIVE_APP_WINDOW_VIEWS_AURA_H_

#include <memory>

#include "base/gtest_prod_util.h"
#include "base/macros.h"
#include "build/build_config.h"
#include "mini_app/core/browser/ui/chrome_native_app_window_views.h"

namespace mini_app {

namespace UI {

// Aura-specific parts of ChromeNativeAppWindowViews. This is used directly on
// Linux and Windows, and is the base class for the Ash specific class used on
// ChromeOS.
class ChromeNativeAppWindowViewsAura : public ChromeNativeAppWindowViews {
 public:
  ChromeNativeAppWindowViewsAura();
  ~ChromeNativeAppWindowViewsAura() override;

 protected:
  ui::WindowShowState GetRestorableState(
      const ui::WindowShowState restore_state) const;

  // ChromeNativeAppWindowViews implementation.
  void OnBeforeWidgetInit(
      const app_window::AppWindow::CreateParams& create_params,
      views::Widget::InitParams* init_params,
      views::Widget* widget) override;
  views::NonClientFrameView* CreateNonStandardAppFrame() override;

  // ui::BaseWindow implementation.
  ui::WindowShowState GetRestoredState() const override;
  ui::ZOrderLevel GetZOrderLevel() const override;

  // NativeAppWindow implementation.
  void UpdateShape(std::unique_ptr<ShapeRects> rects) override;

 private:
  FRIEND_TEST_ALL_PREFIXES(ShapedAppWindowTargeterTest,
                           ResizeInsetsWithinBounds);

  DISALLOW_COPY_AND_ASSIGN(ChromeNativeAppWindowViewsAura);
};

}  // namespace UI

}   // namespace mini_app

#endif  // MINI_APP_CORE_UI_VIEWS_APPS_CHROME_NATIVE_APP_WINDOW_VIEWS_AURA_H_
