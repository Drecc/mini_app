#include "mini_app/core/browser/mini_app_window_client.h"

#include <memory>

#include "base/memory/singleton.h"
#include "build/build_config.h"
#include "mini_app/core/browser/app_window/app_window.h"
#include "mini_app/core/browser/mini_app_window_delegate.h"
#include "mini_app/core/browser/ui/chrome_native_app_window_views_aura.h"

namespace mini_app {

MiniAppWindowClient::MiniAppWindowClient() {
}

MiniAppWindowClient::~MiniAppWindowClient() {
}

// static
MiniAppWindowClient* MiniAppWindowClient::GetInstance() {
  return base::Singleton<
      MiniAppWindowClient,
      base::LeakySingletonTraits<MiniAppWindowClient>>::get();
}

app_window::AppWindow* MiniAppWindowClient::CreateAppWindow(
    content::BrowserContext* context, const std::string& id) {
  return new app_window::AppWindow(context, id, new MiniAppDelegate());
}

app_window::NativeAppWindow* MiniAppWindowClient::CreateNativeAppWindow(
    app_window::AppWindow* window,
    app_window::AppWindow::CreateParams* params) {
  return CreateNativeAppWindowImpl(window, *params);
}

app_window::NativeAppWindow* MiniAppWindowClient::CreateNativeAppWindowImpl(
      app_window::AppWindow* app_window,
      const app_window::AppWindow::CreateParams& params) {
  UI::ChromeNativeAppWindowViewsAura* window = new UI::ChromeNativeAppWindowViewsAura;
  window->Init(app_window, params);
  return window;
}

}
