#include "mini_app/core/browser/mini_app_controller.h"

#include "base/logging.h"

#include "content/public/browser/render_process_host.h"
#include "mini_app/core/application/browser/application_system.h"
#include "mini_app/core/browser/app_window/app_window_contents.h"
#include "mini_app/core/browser/app_window/app_window.h"
#include "mini_app/core/browser/application_component.h"
#include "mini_app/core/browser/mini_app_browser_context.h"
#include "mini_app/core/browser/mini_app_browser_main_parts.h"
#include "mini_app/core/browser/mini_app_window_client.h"
#include "mini_app/core/browser/ui/ui_env_controller_aura.h"

namespace mini_app {

MiniAppController::MiniAppController() {
    LOG(INFO) << "MiniAppController::MiniAppController";
}

MiniAppController::~MiniAppController() {
    LOG(INFO) << "MiniAppController::~MiniAppController";
}

MiniAppController* MiniAppController::GetInstance() {
    return MiniAppBrowserMainParts::GetInstance()->GetMiniAppController();
}

void MiniAppController::SetMainMessageLoopQuitClosure(base::OnceClosure quit_closure) {
    if(app_component_) {
        app_component_->SetApplicationQuitClosure(std::move(quit_closure));
    }
}

void MiniAppController::PreMainMessageLoopRun() {
    browser_context_.reset(new MiniAppBrowserContext);
    desktop_controller_.reset(new UI::UiEnvControllerAura());
    app_window::AppWindowClient::Set(MiniAppWindowClient::GetInstance());
    desktop_controller_->PreMainMessageLoopRun();
    CreateComponents();
}

bool MiniAppController::Launch(GURL start_url) {
    if(app_component_ && app_component_->app_system()) {
        application::ApplicationSystem* app_system = app_component_->app_system();
        return app_system->LaunchFromCommandLine(start_url);
    }
    return false;
}

void MiniAppController::PostMainMessageLoopRun() {
    DestroyComponents();
    desktop_controller_->PostMainMessageLoopRun();
    browser_context_.reset();
}

void MiniAppController::CreateComponents() {
    app_component_.reset(new ApplicationComponent(browser_context_.get()));
}

void MiniAppController::DestroyComponents() {
    app_component_.reset();
}

}   //namespace mini_app