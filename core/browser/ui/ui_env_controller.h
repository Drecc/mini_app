#ifndef MINI_APP_CORE_UI_BROWSER_UI_ENV_CONTROLLER_H_
#define MINI_APP_CORE_UI_BROWSER_UI_ENV_CONTROLLER_H_

namespace mini_app {

namespace app_window {
class AppWindow;
}

namespace UI {

class UiEnvController {
public:
  UiEnvController();
  virtual ~UiEnvController();

  static UiEnvController* instance();

  virtual void PreMainMessageLoopRun() = 0;
  virtual void PostMainMessageLoopRun() = 0;

};

}  // namespace UI

}   // namespace mini_app

#endif  // MINI_APP_CORE_UI_BROWSER_UI_ENV_CONTROLLER_H_
