#include "mini_app/core/application/browser/application_system.h"

#include <string>
#include "base/command_line.h"
#include "base/files/file_util.h"
#include "content/public/browser/render_process_host.h"
#include "content/public/common/content_switches.h"
#include "net/base/filename_util.h"
#include "mini_app/core/application/browser/application.h"
#include "mini_app/core/application/browser/application_service.h"
#include "mini_app/core/application/common/application_manifest_constants.h"
#include "mini_app/core/application/common/id_util.h"
#include "mini_app/core/browser/mini_app_browser_context.h"

namespace mini_app {
namespace application {

ApplicationSystem::ApplicationSystem(MiniAppBrowserContext* browser_context)
  : browser_context_(browser_context),
    application_service_(ApplicationService::Create(
        browser_context)) {}

ApplicationSystem::~ApplicationSystem() {
}

// static
std::unique_ptr<ApplicationSystem> ApplicationSystem::Create(
    MiniAppBrowserContext* browser_context) {
  std::unique_ptr<ApplicationSystem> app_system;
  app_system.reset(new ApplicationSystem(browser_context));
  return app_system;
}

bool ApplicationSystem::LaunchFromCommandLine(const GURL& url) {
  if (!url.is_valid())
    return false;

  base::FilePath path;
  Application* app = nullptr;
  bool is_local = url.SchemeIsFile() && net::FileURLToFilePath(url, &path);
  if (!is_local) {  // Handles external URL.
    app = application_service_->LaunchHostedURL(url);
    return !!app;
  }

  if (!base::PathExists(path))
    return false;

  if (path.MatchesExtension(FILE_PATH_LITERAL(".xpk")) ||
      path.MatchesExtension(FILE_PATH_LITERAL(".wgt"))) {
    app = application_service_->LaunchFromPackagePath(path);
    return !!app;
  }

  if (path.MatchesExtension(FILE_PATH_LITERAL(".json"))) {
    app = application_service_->LaunchFromManifestPath(
        path, Manifest::TYPE_MANIFEST);
    return !!app;
  }

  if (path.MatchesExtension(FILE_PATH_LITERAL(".xml"))) {
    app = application_service_->LaunchFromManifestPath(
        path, Manifest::TYPE_WIDGET);
    return !!app;
  }

  return false;
}

void ApplicationSystem::SetMainMessageLoopQuitClosure(base::OnceClosure quit_closure) {
  application_service_->SetMainMessageLoopQuitClosure(std::move(quit_closure));
}

}  // namespace application
}  // namespace mini_app
