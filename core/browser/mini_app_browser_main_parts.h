#ifndef MINI_APP_BROWSER_MINI_APP_BROWSER_MAIN_PARTS_H_
#define MINI_APP_BROWSER_MINI_APP_BROWSER_MAIN_PARTS_H_

#include "content/public/browser/browser_main_parts.h"
#include "content/public/common/main_function_params.h"
#include "mini_app/core/browser/mini_app_controller.h"
#include "url/gurl.h"

namespace mini_app {
class MiniAppController;

class MiniAppBrowserMainParts : public content::BrowserMainParts {
public:
    MiniAppBrowserMainParts(const content::MainFunctionParams& parameters);
    ~MiniAppBrowserMainParts() override;

    static MiniAppBrowserMainParts* GetInstance();

    MiniAppController* GetMiniAppController() {
      return mini_app_controller_.get();
    }

    //content::BrowserMainParts
    int PreEarlyInitialization() override;
    void PostEarlyInitialization() override;
    void PreMainMessageLoopStart() override;
    void PostMainMessageLoopStart() override;
    void ToolkitInitialized() override;
    int PreCreateThreads() override;
    void PostCreateThreads() override;
    void PreMainMessageLoopRun() override;
    bool MainMessageLoopRun(int* result_code)override;
    void PreDefaultMainMessageLoopRun(base::OnceClosure quit_closure) override;
    void PostMainMessageLoopRun() override;
    void PostDestroyThreads() override;

protected:

private:
    const content::MainFunctionParams parameters_;
    GURL startup_url_;
    std::unique_ptr<MiniAppController> mini_app_controller_;
    bool run_default_message_loop_;
    static MiniAppBrowserMainParts* self_;

    DISALLOW_COPY_AND_ASSIGN(MiniAppBrowserMainParts);
};

}  //namespace mini_app


#endif //  MINI_APP_BROWSER_MINI_APP_BROWSER_MAIN_PARTS_H_