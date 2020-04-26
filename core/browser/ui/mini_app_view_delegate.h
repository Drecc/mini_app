#ifndef MINI_APP_CORE_UI_VIEWS_CHROME_VIEWS_DELEGATE_H_
#define MINI_APP_CORE_UI_VIEWS_CHROME_VIEWS_DELEGATE_H_

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/macros.h"
#include "build/build_config.h"
#include "ui/views/views_delegate.h"

namespace mini_app {

namespace UI {

class MiniAppViewsDelegate : public views::ViewsDelegate {
 public:
  MiniAppViewsDelegate();
  ~MiniAppViewsDelegate() override;

  // views::ViewsDelegate: 
  void OnBeforeWidgetInit(
      views::Widget::InitParams* params,
      views::internal::NativeWidgetDelegate* delegate) override;
  ui::ContextFactory* GetContextFactory() override;
  ui::ContextFactoryPrivate* GetContextFactoryPrivate() override;

 private:
  views::Widget::InitParams::WindowOpacity GetOpacityForInitParams(
      const views::Widget::InitParams& params);

  views::NativeWidget* CreateNativeWidget(
      views::Widget::InitParams* params,
      views::internal::NativeWidgetDelegate* delegate);


  DISALLOW_COPY_AND_ASSIGN(MiniAppViewsDelegate);
};

}  // namespace UI

}   // namespace mini_app

#endif  // MINI_APP_CORE_UI_VIEWS_CHROME_VIEWS_DELEGATE_H_
