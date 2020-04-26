#ifndef MINI_APP_CORE_UI_BROWSER_UI_ENV_CONTROLLER_AURA_H_
#define MINI_APP_CORE_UI_BROWSER_UI_ENV_CONTROLLER_AURA_H_

#include "base/macros.h"
#include "mini_app/core/browser/ui/ui_env_controller.h"
#include "ui/display/display.h"

namespace display {
class Screen;
}

namespace wm {
class CompoundEventFilter;
class FocusController;
class WMState;
}

namespace views {
class ViewsDelegate;
}

namespace mini_app {

namespace app_window {
class AppWindow;
}

namespace UI {

class UiEnvControllerAura : public UiEnvController {
public:
  explicit UiEnvControllerAura();
  ~UiEnvControllerAura() override;

  // UiEnvControllerAura:
  void PreMainMessageLoopRun() override;
  void PostMainMessageLoopRun() override;

protected:
  virtual void InitWindowManager();
  virtual void TearDownWindowManager();

private:
  //todo zengmingcong
  void MaybeQuit();

  std::unique_ptr<display::Screen> screen_;
  std::unique_ptr<views::ViewsDelegate> views_delegate_;
  std::unique_ptr<wm::CompoundEventFilter> root_window_event_filter_;
  std::unique_ptr<wm::FocusController> focus_controller_;
  std::unique_ptr<wm::WMState> wm_state_;

  DISALLOW_COPY_AND_ASSIGN(UiEnvControllerAura);
};

}  // namespace UI

}   // namespace mini_app

#endif  // MINI_APP_CORE_UI_BROWSER_UI_ENV_CONTROLLER_AURA_H_
