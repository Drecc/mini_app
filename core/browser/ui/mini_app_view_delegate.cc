#include "mini_app/core/browser/ui/mini_app_view_delegate.h"

#include "base/logging.h"
#include "build/build_config.h"
#include "content/public/browser/context_factory.h"
#include "ui/display/display.h"
#include "ui/display/screen.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/views/widget/desktop_aura/desktop_native_widget_aura.h"
#include "ui/views/widget/native_widget_aura.h"
#include "ui/views/widget/widget.h"

namespace mini_app {

namespace UI {

MiniAppViewsDelegate::MiniAppViewsDelegate() {
}

MiniAppViewsDelegate::~MiniAppViewsDelegate() {

}

void MiniAppViewsDelegate::OnBeforeWidgetInit(
    views::Widget::InitParams* params,
    views::internal::NativeWidgetDelegate* delegate) {
  // We need to determine opacity if it's not already specified.
  if (params->opacity == views::Widget::InitParams::WindowOpacity::kInferred)
    params->opacity = GetOpacityForInitParams(*params);

  // If we already have a native_widget, we don't have to try to come
  // up with one.
  if (params->native_widget)
    return;

  if (!native_widget_factory().is_null()) {
    params->native_widget = native_widget_factory().Run(*params, delegate);
    if (params->native_widget)
      return;
  }

  params->native_widget = CreateNativeWidget(params, delegate);
}

ui::ContextFactory* MiniAppViewsDelegate::GetContextFactory() {
  return content::GetContextFactory();
}

ui::ContextFactoryPrivate* MiniAppViewsDelegate::GetContextFactoryPrivate() {
  return content::GetContextFactoryPrivate();
}

views::Widget::InitParams::WindowOpacity
MiniAppViewsDelegate::GetOpacityForInitParams(
    const views::Widget::InitParams& params) {
  return views::Widget::InitParams::WindowOpacity::kOpaque;
}

views::NativeWidget* MiniAppViewsDelegate::CreateNativeWidget(
    views::Widget::InitParams* params,
    views::internal::NativeWidgetDelegate* delegate) {
  if (params->parent &&
      params->type != views::Widget::InitParams::TYPE_MENU &&
      params->type != views::Widget::InitParams::TYPE_TOOLTIP) {
    return new views::NativeWidgetAura(delegate);
  } else {
    return new views::DesktopNativeWidgetAura(delegate);
  }
}

}  // namespace UI

}   // namespace mini_app