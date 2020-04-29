#include "mini_app/core/app/mini_app_main_delegate.h"

#include "base/logging.h"
#include "base/path_service.h"
#include "mini_app/core/browser/mini_app_content_browser_client.h"
#include "mini_app/core/common/mini_app_content_client.h"
#include "mini_app/core/renderer/mini_app_content_renderer_client.h"
#include "ui/base/resource/resource_bundle.h"

namespace mini_app {

MiniAppMainDelegate::MiniAppMainDelegate() {

}

MiniAppMainDelegate::~MiniAppMainDelegate() {
    
}

bool MiniAppMainDelegate::BasicStartupComplete(int* exit_code) {
    *exit_code = 0;
    content_client_.reset(new MiniAppContentClient);
    SetContentClient(content_client_.get());
    return false;
}

void MiniAppMainDelegate::PreSandboxStartup() {
    InitializeResourceBundle();
}

void MiniAppMainDelegate::SandboxInitialized(const std::string& process_type) {
}

int MiniAppMainDelegate::RunProcess(const std::string& process_type, const content::MainFunctionParams& main_function_params) {
    if(!process_type.empty())
        return -1;
    return -1;
}

void MiniAppMainDelegate::InitializeResourceBundle() {
    base::FilePath pak_file;
    bool r = base::PathService::Get(base::DIR_MODULE, &pak_file);
    CHECK(r) << "pak_file miss";
    pak_file = pak_file.Append(FILE_PATH_LITERAL("mini_app.pak"));
    ui::ResourceBundle::InitSharedInstanceWithPakPath(pak_file);
}

content::ContentBrowserClient* MiniAppMainDelegate::CreateContentBrowserClient() {
    return new MiniAppContentBrowserClient();
}

content::ContentGpuClient* MiniAppMainDelegate::CreateContentGpuClient() {
    return content::ContentMainDelegate::CreateContentGpuClient();
}

content::ContentRendererClient* MiniAppMainDelegate::CreateContentRendererClient() {
    return new MiniAppContentRendererClient();
}

content::ContentUtilityClient* MiniAppMainDelegate::CreateContentUtilityClient() {
    return content::ContentMainDelegate::CreateContentUtilityClient();    
}


}   //namespace mini_app