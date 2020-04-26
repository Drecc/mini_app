#include "mini_app/core/browser/mini_app_browser_main_parts.h"

#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "base/logging.h"
#include "base/message_loop/message_loop.h"
#include "mini_app/core/browser/mini_app_controller.h"
#include "net/base/filename_util.h"
#include "services/service_manager/embedder/result_codes.h"
#include "ui/base/ime/init/input_method_initializer.h"

namespace {

GURL GetURLFromCommandLine(const base::CommandLine& command_line) {
  const base::CommandLine::StringVector& args = command_line.GetArgs();

  if (args.empty())
    return GURL();

  GURL url(args[0]);
  if (url.is_valid() && url.has_scheme())
    return url;

  base::FilePath path(args[0]);
  if (!path.IsAbsolute())
    path = MakeAbsoluteFilePath(path);

  return net::FilePathToFileURL(path);
}

}


namespace mini_app {

MiniAppBrowserMainParts* MiniAppBrowserMainParts::self_ = nullptr;

MiniAppBrowserMainParts::MiniAppBrowserMainParts(const content::MainFunctionParams& parameters)
    : parameters_(parameters)
    , mini_app_controller_(new MiniAppController)
    , run_default_message_loop_(true) {
    CHECK(!self_) << "only one MiniAppBrowserMainParts";
    self_ = this;
}

MiniAppBrowserMainParts::~MiniAppBrowserMainParts() {
}

MiniAppBrowserMainParts* MiniAppBrowserMainParts::GetInstance() {
    CHECK(self_);
    return self_;
}
int MiniAppBrowserMainParts::PreEarlyInitialization() {
    ui::InitializeInputMethodForTesting();
    return service_manager::RESULT_CODE_NORMAL_EXIT;
}

void MiniAppBrowserMainParts::PostEarlyInitialization() {
}

void MiniAppBrowserMainParts::PreMainMessageLoopStart() {
}

void MiniAppBrowserMainParts::PostMainMessageLoopStart() {
}

void MiniAppBrowserMainParts::ToolkitInitialized() {
}

int MiniAppBrowserMainParts::PreCreateThreads() {
    return 0;
}

void MiniAppBrowserMainParts::PostCreateThreads() { 
}

void MiniAppBrowserMainParts::PreMainMessageLoopRun() {
    base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
    MiniAppController::GetInstance()->PreMainMessageLoopRun();

    startup_url_ = GetURLFromCommandLine(*command_line);
    run_default_message_loop_ = MiniAppController::GetInstance()->Launch(startup_url_);
    MiniAppController::GetInstance()->Launch(GURL("https://baidu.com"));

    if (parameters_.ui_task) {
        std::move(*parameters_.ui_task).Run();
        delete parameters_.ui_task;
        run_default_message_loop_ = false;
    }
}

bool MiniAppBrowserMainParts::MainMessageLoopRun(int* result_code) {
    return !run_default_message_loop_;
}

void MiniAppBrowserMainParts::PreDefaultMainMessageLoopRun(base::OnceClosure quit_closure) {
    MiniAppController::GetInstance()->SetMainMessageLoopQuitClosure(std::move(quit_closure));    
}

void MiniAppBrowserMainParts::PostMainMessageLoopRun() {
    MiniAppController::GetInstance()->PostMainMessageLoopRun(); 
}

void MiniAppBrowserMainParts::PostDestroyThreads() {
}

}   //namespace mini_app