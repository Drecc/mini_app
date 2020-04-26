#include "mini_app/core/browser/application_component.h"
#include "mini_app/core/application/browser/application_system.h"

namespace mini_app {

ApplicationComponent::ApplicationComponent(
    MiniAppBrowserContext* browser_context)
    : app_system_(application::ApplicationSystem::Create(browser_context)) {
}

ApplicationComponent::~ApplicationComponent() {}

void ApplicationComponent::SetApplicationQuitClosure(base::OnceClosure quit_closure) {
    app_system_->SetMainMessageLoopQuitClosure(std::move(quit_closure));
}

}  // namespace mini_app
