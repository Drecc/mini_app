#ifndef MINI_APP_APP_MINI_APP_MAIN_DELEGATE_H_
#define MINI_APP_APP_MINI_APP_MAIN_DELEGATE_H_

#include "content/public/app/content_main_delegate.h"

namespace content {
class ContentBrowserClient;
class ContentGpuClient;
class ContentRendererClient;
class ContentUtilityClient;
class ContentClient;
}

namespace mini_app {

class MiniAppMainDelegate : public content::ContentMainDelegate {
public:
    MiniAppMainDelegate();
    ~MiniAppMainDelegate() override;

    //content::ContentMainDelegate
    bool BasicStartupComplete(int* exit_code) override;
    void PreSandboxStartup() override;
    void SandboxInitialized(const std::string& process_type) override;
    int RunProcess(
        const std::string& process_type,
        const content::MainFunctionParams& main_function_params) override;

protected:
    void InitializeResourceBundle();

    //content::ContentMainDelegate
    content::ContentBrowserClient* CreateContentBrowserClient() override;
    content::ContentGpuClient* CreateContentGpuClient() override;
    content::ContentRendererClient* CreateContentRendererClient() override;
    content::ContentUtilityClient* CreateContentUtilityClient() override;

private:
    std::unique_ptr<content::ContentClient> content_client_;

};

}  //namespace mini_app


#endif //  MINI_APP_APP_MINI_APP_MAIN_DELEGATE_H_