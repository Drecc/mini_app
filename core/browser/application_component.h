#ifndef MINI_APP_APP_MINI_APP_APPLICATION_COMPONENT_H_
#define MINI_APP_APP_MINI_APP_APPLICATION_COMPONENT_H_

#include <memory>

#include "base/callback_forward.h"
#include "base/message_loop/message_loop.h"
#include "mini_app/core/browser/mini_app_component.h"

namespace mini_app {

class MiniAppBrowserContext;

namespace application {
class ApplicationSystem;
}

class ApplicationComponent : public MiniAppComponent {
public:
    explicit ApplicationComponent(MiniAppBrowserContext* browser_context);
    ~ApplicationComponent() override;

    application::ApplicationSystem* app_system() { return app_system_.get(); }
    void SetApplicationQuitClosure(base::OnceClosure quit_closure);
private:
    // MiniAppComponent

    std::unique_ptr<application::ApplicationSystem> app_system_;
};

}  // namespace mini_app

#endif  // MINI_APP_APP_MINI_APP_APPLICATION_COMPONENT_H_
