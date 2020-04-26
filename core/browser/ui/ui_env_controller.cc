#include "mini_app/core/browser/ui/ui_env_controller.h"

#include "base/logging.h"
#include "base/macros.h"

namespace mini_app {

namespace app_window {
class AppWindow;
}

namespace UI {

namespace {

UiEnvController* g_instance = nullptr;

}  // namespace

// static
UiEnvController* UiEnvController::instance() {
  return g_instance;
}

UiEnvController::UiEnvController() {
  CHECK(!g_instance) << "UiEnvController already exists";
  g_instance = this;
}

UiEnvController::~UiEnvController() {
  DCHECK(g_instance);
  g_instance = NULL;
}

}  // namespace UI

}   // namespace mini_app
