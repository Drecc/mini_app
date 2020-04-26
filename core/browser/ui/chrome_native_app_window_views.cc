#include "mini_app/core/browser/ui/chrome_native_app_window_views.h"

#include <stddef.h>
#include <utility>

#include "apps/ui/views/app_window_frame_view.h"
#include "base/no_destructor.h"
#include "base/stl_util.h"
#include "build/build_config.h"
// #include "chrome/app/chrome_command_ids.h"
// #include "chrome/browser/app_mode/app_mode_utils.h"
// #include "chrome/browser/extensions/chrome_app_icon.h"
// #include "chrome/browser/extensions/chrome_app_icon_service.h"
// #include "chrome/browser/extensions/extension_util.h"
// #include "chrome/browser/profiles/profile.h"
// #include "chrome/browser/ui/views/accelerator_table.h"
// #include "chrome/browser/ui/views/extensions/extension_keybinding_registry_views.h"
#include "components/favicon/content/content_favicon_driver.h"
#include "components/zoom/page_zoom.h"
#include "components/zoom/zoom_controller.h"
#include "mini_app/core/browser/app_window/app_delegate.h"
#include "ui/gfx/image/image_skia_operations.h"
#include "ui/gfx/skia_util.h"
#include "ui/views/controls/webview/webview.h"
#include "ui/views/widget/widget.h"

namespace mini_app {

namespace UI {

using app_window::AppWindow;

ChromeNativeAppWindowViews::ChromeNativeAppWindowViews()
    : has_frame_color_(false),
      active_frame_color_(SK_ColorBLACK),
      inactive_frame_color_(SK_ColorBLACK) {
}

ChromeNativeAppWindowViews::~ChromeNativeAppWindowViews() {}

void ChromeNativeAppWindowViews::OnBeforeWidgetInit(
    const AppWindow::CreateParams& create_params,
    views::Widget::InitParams* init_params,
    views::Widget* widget) {
}

void ChromeNativeAppWindowViews::InitializeDefaultWindow(
    const AppWindow::CreateParams& create_params) {
  views::Widget::InitParams init_params(views::Widget::InitParams::TYPE_WINDOW);
  init_params.delegate = this;
  init_params.remove_standard_frame = ShouldRemoveStandardFrame();
  init_params.use_system_default_icon = true;
  if (create_params.alpha_enabled) {
    init_params.opacity =
        views::Widget::InitParams::WindowOpacity::kTranslucent;

    // The given window is most likely not rectangular since it uses
    // transparency and has no standard frame, don't show a shadow for it.
    // TODO(skuhne): If we run into an application which should have a shadow
    // but does not have, a new attribute has to be added.
    if (IsFrameless())
      init_params.shadow_type = views::Widget::InitParams::ShadowType::kNone;
  }
  if (create_params.always_on_top)
    init_params.z_order = ui::ZOrderLevel::kFloatingWindow;
  init_params.visible_on_all_workspaces =
      create_params.visible_on_all_workspaces;

  OnBeforeWidgetInit(create_params, &init_params, widget());
  widget()->Init(std::move(init_params));

  // The frame insets are required to resolve the bounds specifications
  // correctly. So we set the window bounds and constraints now.
  gfx::Insets frame_insets = GetFrameInsets();
  gfx::Rect window_bounds = create_params.GetInitialWindowBounds(frame_insets);
  SetContentSizeConstraints(create_params.GetContentMinimumSize(frame_insets),
                            create_params.GetContentMaximumSize(frame_insets));
  if (!window_bounds.IsEmpty()) {
    using BoundsSpecification = AppWindow::BoundsSpecification;
    bool position_specified =
        window_bounds.x() != BoundsSpecification::kUnspecifiedPosition &&
        window_bounds.y() != BoundsSpecification::kUnspecifiedPosition;
    if (!position_specified)
      widget()->CenterWindow(window_bounds.size());
    else
      widget()->SetBounds(window_bounds);
  }
}

views::NonClientFrameView*
ChromeNativeAppWindowViews::CreateStandardDesktopAppFrame() {
  return views::WidgetDelegateView::CreateNonClientFrameView(widget());
}

bool ChromeNativeAppWindowViews::ShouldRemoveStandardFrame() {
  return IsFrameless() || has_frame_color_;
}

// ui::BaseWindow implementation.

gfx::Rect ChromeNativeAppWindowViews::GetRestoredBounds() const {
  return widget()->GetRestoredBounds();
}

ui::WindowShowState ChromeNativeAppWindowViews::GetRestoredState() const {
  if (IsMaximized())
    return ui::SHOW_STATE_MAXIMIZED;
  if (IsFullscreen())
    return ui::SHOW_STATE_FULLSCREEN;

  return ui::SHOW_STATE_NORMAL;
}

ui::ZOrderLevel ChromeNativeAppWindowViews::GetZOrderLevel() const {
  return widget()->GetZOrderLevel();
}

// views::WidgetDelegate implementation.

gfx::ImageSkia ChromeNativeAppWindowViews::GetWindowAppIcon() {
  return gfx::ImageSkia();
}

gfx::ImageSkia ChromeNativeAppWindowViews::GetWindowIcon() {
  // Resulting icon is cached in aura::client::kWindowIconKey window property.
  // content::WebContents* web_contents = app_window()->web_contents();
  // if (web_contents) {
  //   favicon::FaviconDriver* favicon_driver =
  //       favicon::ContentFaviconDriver::FromWebContents(web_contents);
  //   gfx::Image app_icon = favicon_driver->GetFavicon();
  //   if (!app_icon.IsEmpty())
  //     return *app_icon.ToImageSkia();
  // }
  return gfx::ImageSkia();
}

views::NonClientFrameView* ChromeNativeAppWindowViews::CreateNonClientFrameView(
    views::Widget* widget) {
  return (IsFrameless() || has_frame_color_) ?
      CreateNonStandardAppFrame() : CreateStandardDesktopAppFrame();
}

bool ChromeNativeAppWindowViews::WidgetHasHitTestMask() const {
  return shape_ != NULL;
}

void ChromeNativeAppWindowViews::GetWidgetHitTestMask(SkPath* mask) const {
  shape_->getBoundaryPath(mask);
}

// views::View implementation.

bool ChromeNativeAppWindowViews::AcceleratorPressed(
    const ui::Accelerator& accelerator) {
  return NativeAppWindowViews::AcceleratorPressed(accelerator);
}

// NativeAppWindow implementation.

void ChromeNativeAppWindowViews::SetFullscreen(int fullscreen_types) {
  widget()->SetFullscreen(fullscreen_types != AppWindow::FULLSCREEN_TYPE_NONE);
}

bool ChromeNativeAppWindowViews::IsFullscreenOrPending() const {
  return widget()->IsFullscreen();
}

void ChromeNativeAppWindowViews::UpdateShape(
    std::unique_ptr<ShapeRects> rects) {
  shape_rects_ = std::move(rects);

  // Build a region from the list of rects when it is supplied.
  std::unique_ptr<SkRegion> region;
  if (shape_rects_) {
    region = std::make_unique<SkRegion>();
    for (const gfx::Rect& input_rect : *shape_rects_.get())
      region->op(gfx::RectToSkIRect(input_rect), SkRegion::kUnion_Op);
  }
  shape_ = std::move(region);
  widget()->SetShape(shape() ? std::make_unique<ShapeRects>(*shape_rects_)
                             : nullptr);
  widget()->OnSizeConstraintsChanged();
}

bool ChromeNativeAppWindowViews::HasFrameColor() const {
  return has_frame_color_;
}

SkColor ChromeNativeAppWindowViews::ActiveFrameColor() const {
  return active_frame_color_;
}

SkColor ChromeNativeAppWindowViews::InactiveFrameColor() const {
  return inactive_frame_color_;
}

// NativeAppWindowViews implementation.

void ChromeNativeAppWindowViews::InitializeWindow(
    AppWindow* app_window,
    const AppWindow::CreateParams& create_params) {
  DCHECK(widget());
  has_frame_color_ = create_params.has_frame_color;
  active_frame_color_ = create_params.active_frame_color;
  inactive_frame_color_ = create_params.inactive_frame_color;
  InitializeDefaultWindow(create_params);
}

}  // namespace UI

}   // namespace mini_app