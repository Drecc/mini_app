#include "mini_app/core/app/mini_app_main_delegate.h"

#include "base/command_line.h"
#include "base/logging.h"
#include "base/path_service.h"
#include "content/public/common/content_switches.h"
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
#if defined(OS_WIN)
    //为了看log 所以以进程的方式启动
    base::CommandLine* line = base::CommandLine::ForCurrentProcess();
    line->AppendSwitch(switches::kSingleProcess);
#endif

    *exit_code = 0;
    content_client_.reset(new MiniAppContentClient);
    SetContentClient(content_client_.get());
    return false;
}

void MiniAppMainDelegate::PreSandboxStartup() {
    InitializeResourceBundle();

    logging::LoggingSettings log_settings;
    log_settings.logging_dest = logging::LOG_TO_SYSTEM_DEBUG_LOG | logging::LOG_TO_STDERR;
    logging::InitLogging(log_settings);
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