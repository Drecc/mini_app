#ifndef MINI_APP_CORE_BROWSER_APP_WINDOW_APP_WINDOW_CLIENT_H_
#define MINI_APP_CORE_BROWSER_APP_WINDOW_APP_WINDOW_CLIENT_H_

#include "base/callback_forward.h"
#include "mini_app/core/browser/app_window/app_window.h"

namespace content {
class BrowserContext;
class WebContents;
}

namespace mini_app {

namespace app_window {

class NativeAppWindow;

class AppWindowClient {
 public:
  virtual ~AppWindowClient() {}

  static AppWindowClient* Get();
  static void Set(AppWindowClient* client);

  virtual AppWindow* CreateAppWindow(content::BrowserContext* context, const std::string& id) = 0;
  virtual NativeAppWindow* CreateNativeAppWindow(
      AppWindow* window,
      AppWindow::CreateParams* params) = 0;
};

}  // namespace app_window

}   // namespace mini_app

#endif  // MINI_APP_CORE_BROWSER_APP_WINDOW_APP_WINDOW_CLIENT_H_
