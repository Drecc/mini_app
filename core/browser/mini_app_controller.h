#ifndef MINI_APP_APP_MINI_APP_CONTROLLER_H_
#define MINI_APP_APP_MINI_APP_CONTROLLER_H_

#include "mini_app/core/browser/mini_app_browser_main_parts.h"
#include "mini_app/core/browser/mini_app_component.h"
#include "url/gurl.h"

namespace mini_app {
class ApplicationComponent;
class MiniAppBrowserContext;

namespace UI {
class UiEnvController;
}
namespace application {
class ApplicationSystem;
}

class MiniAppController {
public:
    virtual ~MiniAppController();
    static MiniAppController* GetInstance();

    void SetMainMessageLoopQuitClosure(base::OnceClosure quit_closure);
    void PreMainMessageLoopRun();
    bool Launch(GURL start_url);
    void PostMainMessageLoopRun();
    MiniAppBrowserContext* browser_context() { return browser_context_.get(); }

protected:
    MiniAppController();

    virtual void CreateComponents();
    virtual void DestroyComponents();

private:
    friend class MiniAppBrowserMainParts;

    base::OnceClosure quit_main_message_loop_;
    std::unique_ptr<MiniAppBrowserContext> browser_context_;
    std::unique_ptr<ApplicationComponent> app_component_;
    std::unique_ptr<UI::UiEnvController> desktop_controller_;

    DISALLOW_COPY_AND_ASSIGN(MiniAppController);
};

}  //namespace mini_app


#endif //  MINI_APP_APP_MINI_APP_CONTROLLER_H_