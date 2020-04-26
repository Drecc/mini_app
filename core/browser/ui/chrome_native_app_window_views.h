#ifndef MINI_APP_CORE_BROWSER_UI_VIEWS_APPS_CHROME_NATIVE_APP_WINDOW_VIEWS_H_
#define MINI_APP_CORE_BROWSER_UI_VIEWS_APPS_CHROME_NATIVE_APP_WINDOW_VIEWS_H_

#include <memory>

#include "base/macros.h"
#include "mini_app/core/browser/ui/native_app_window_views.h"

namespace mini_app {

namespace UI {

class ChromeNativeAppWindowViews : public NativeAppWindowViews {
 public:
  ChromeNativeAppWindowViews();
  ~ChromeNativeAppWindowViews() override;

  SkRegion* shape() { return shape_.get(); }
  ShapeRects* shape_rects() { return shape_rects_.get(); }

 protected:
  // Called before views::Widget::Init() in InitializeDefaultWindow() to allow
  // subclasses to customize the InitParams that would be passed.
  virtual void OnBeforeWidgetInit(
      const app_window::AppWindow::CreateParams& create_params,
      views::Widget::InitParams* init_params,
      views::Widget* widget);
  virtual void InitializeDefaultWindow(
      const app_window::AppWindow::CreateParams& create_params);
  virtual views::NonClientFrameView* CreateStandardDesktopAppFrame();
  virtual views::NonClientFrameView* CreateNonStandardAppFrame() = 0;
  virtual bool ShouldRemoveStandardFrame();

  // ui::BaseWindow implementation.
  gfx::Rect GetRestoredBounds() const override;
  ui::WindowShowState GetRestoredState() const override;
  ui::ZOrderLevel GetZOrderLevel() const override;

  // WidgetDelegate implementation.
  gfx::ImageSkia GetWindowAppIcon() override;
  gfx::ImageSkia GetWindowIcon() override;
  views::NonClientFrameView* CreateNonClientFrameView(
      views::Widget* widget) override;
  bool WidgetHasHitTestMask() const override;
  void GetWidgetHitTestMask(SkPath* mask) const override;

  // views::View implementation.
  bool AcceleratorPressed(const ui::Accelerator& accelerator) override;

  // NativeAppWindow implementation.
  void SetFullscreen(int fullscreen_types) override;
  bool IsFullscreenOrPending() const override;
  void UpdateShape(std::unique_ptr<ShapeRects> rects) override;
  bool HasFrameColor() const override;
  SkColor ActiveFrameColor() const override;
  SkColor InactiveFrameColor() const override;

  // NativeAppWindowViews implementation.
  void InitializeWindow(
      app_window::AppWindow* app_window,
      const app_window::AppWindow::CreateParams& create_params) override;

 private:
  // Custom shape of the window. If this is not set then the window has a
  // default shape, usually rectangular.
  std::unique_ptr<SkRegion> shape_;

  std::unique_ptr<ShapeRects> shape_rects_;

  bool has_frame_color_;
  SkColor active_frame_color_;
  SkColor inactive_frame_color_;

  DISALLOW_COPY_AND_ASSIGN(ChromeNativeAppWindowViews);
};

}  // namespace UI

}   // namespace mini_app

#endif  // MINI_APP_CORE_BROWSER_UI_VIEWS_APPS_CHROME_NATIVE_APP_WINDOW_VIEWS_H_
