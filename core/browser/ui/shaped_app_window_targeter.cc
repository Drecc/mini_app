#include "mini_app/core/browser/ui/shaped_app_window_targeter.h"

#include <memory>

#include "mini_app/core/browser/ui/chrome_native_app_window_views.h"

namespace mini_app {

namespace UI {

ShapedAppWindowTargeter::ShapedAppWindowTargeter(
    ChromeNativeAppWindowViews* app_window)
    : app_window_(app_window) {}

ShapedAppWindowTargeter::~ShapedAppWindowTargeter() {}

std::unique_ptr<aura::WindowTargeter::HitTestRects>
ShapedAppWindowTargeter::GetExtraHitTestShapeRects(aura::Window* target) const {
  if (!app_window_->shape_rects())
    return nullptr;

  auto shape_rects = std::make_unique<aura::WindowTargeter::HitTestRects>(
      *app_window_->shape_rects());
  return shape_rects;
}

}  // namespace UI

}   // namespace mini_app