#include "mini_app/core/browser/app_window/app_window_client.h"

#include "base/logging.h"

namespace mini_app {

namespace app_window {

namespace {

AppWindowClient* g_app_window_client = nullptr;

}  // namespace

AppWindowClient* AppWindowClient::Get() {
  return g_app_window_client;
}

void AppWindowClient::Set(AppWindowClient* client) {
  if (g_app_window_client && client) {
    DCHECK_EQ(g_app_window_client, client)
        << "AppWindowClient::Set called with different non-null pointers twice "
        << "in a row. A previous test may have set this without clearing it.";
    return;
  }

  g_app_window_client = client;
}

}  // namespace app_window

} //namespace mini_app
