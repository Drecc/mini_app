#ifndef MINI_APP_CORE_BROWSER_UI_APPS_CHROME_APP_WINDOW_CLIENT_H_
#define MINI_APP_CORE_BROWSER_UI_APPS_CHROME_APP_WINDOW_CLIENT_H_

#include "base/compiler_specific.h"
#include "base/macros.h"
#include "build/build_config.h"
#include "mini_app/core/browser/app_window/app_window_client.h"

namespace base {
template <typename T>
struct DefaultSingletonTraits;
}

namespace mini_app {

// The implementation of AppWindowClient for Chrome.
class MiniAppWindowClient : public app_window::AppWindowClient {
 public:
  MiniAppWindowClient();
  ~MiniAppWindowClient() override;

  // Get the LazyInstance for MiniAppWindowClient.
  static MiniAppWindowClient* GetInstance();

  // app_window::AppWindowClient
  app_window::AppWindow* CreateAppWindow(
      content::BrowserContext* context,
      const std::string& id) override;
  app_window::NativeAppWindow* CreateNativeAppWindow(
      app_window::AppWindow* window,
      app_window::AppWindow::CreateParams* params) override;

 private:
  friend struct base::DefaultSingletonTraits<MiniAppWindowClient>;

  // Implemented in platform specific code.
  static app_window::NativeAppWindow* CreateNativeAppWindowImpl(
      app_window::AppWindow* app_window,
      const app_window::AppWindow::CreateParams& params);

  DISALLOW_COPY_AND_ASSIGN(MiniAppWindowClient);
};

}

#endif  // MINI_APP_CORE_BROWSER_UI_APPS_CHROME_APP_WINDOW_CLIENT_H_
