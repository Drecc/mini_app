#include "mini_app/core/browser/ui/ui_env_controller_aura.h"

#include "base/logging.h"
#include "content/public/browser/context_factory.h"
#include "mini_app/core/browser/app_window/app_window.h"
#include "mini_app/core/browser/app_window/native_app_window.h"
#include "mini_app/core/browser/ui/mini_app_view_delegate.h"
#include "ui/aura/env.h"
#include "ui/base/material_design/material_design_controller.h"
#include "ui/display/screen.h"
#include "ui/views/widget/desktop_aura/desktop_screen.h"
#include "ui/wm/core/base_focus_rules.h"
#include "ui/wm/core/compound_event_filter.h"
#include "ui/wm/core/focus_controller.h"
#include "ui/wm/core/wm_state.h"

namespace mini_app {

namespace UI {

namespace {

class MiniAppFocusRules : public wm::BaseFocusRules {
 public:
  MiniAppFocusRules() {}
  ~MiniAppFocusRules() override {}

  bool SupportsChildActivation(const aura::Window* window) const override {
    return true;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(MiniAppFocusRules);
};

}  // namespace

UiEnvControllerAura::UiEnvControllerAura() {
}

UiEnvControllerAura::~UiEnvControllerAura() {
}

void UiEnvControllerAura::PreMainMessageLoopRun() {
  aura::Env::GetInstance()->set_context_factory(content::GetContextFactory());
  aura::Env::GetInstance()->set_context_factory_private(
      content::GetContextFactoryPrivate());

  if (!views::ViewsDelegate::GetInstance())
      views_delegate_ = std::make_unique<UI::MiniAppViewsDelegate>();

  ui::MaterialDesignController::Initialize();
  wm_state_ = std::make_unique<wm::WMState>();
  InitWindowManager();
}

void UiEnvControllerAura::PostMainMessageLoopRun() {
  TearDownWindowManager();
}

void UiEnvControllerAura::InitWindowManager() {
  root_window_event_filter_ = std::make_unique<wm::CompoundEventFilter>();

  if (!display::Screen::GetScreen()) {
    screen_.reset(views::CreateDesktopScreen());
    display::Screen::SetScreenInstance(screen_.get());
  }

  focus_controller_ =
      std::make_unique<wm::FocusController>(new MiniAppFocusRules());
}

void UiEnvControllerAura::TearDownWindowManager() {
  focus_controller_.reset();
  if (screen_) {
    display::Screen::SetScreenInstance(nullptr);
    screen_.reset();
  }
  root_window_event_filter_.reset();
}

void UiEnvControllerAura::MaybeQuit() {

}

}  // namespace UI

}   // namespace mini_app
