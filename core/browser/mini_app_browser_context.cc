#include "mini_app/core/browser/mini_app_browser_context.h"

#include "base/environment.h"
#include "base/logging.h"
#include "base/nix/xdg_util.h"
#include "base/path_service.h"
#include "content/public/browser/resource_context.h"
#include "content/public/browser/web_contents.h"

namespace mini_app {

class MiniAppResourceContext : public content::ResourceContext {
public:
    MiniAppResourceContext() = default;
    ~MiniAppResourceContext() override {}

private:
    DISALLOW_COPY_AND_ASSIGN(MiniAppResourceContext);
};

MiniAppBrowserContext* g_browser_context = nullptr;

MiniAppBrowserContext::MiniAppBrowserContext()
    : resource_context_(new MiniAppResourceContext()) {
    DCHECK(!g_browser_context);
    g_browser_context = this;
    InitPath();
}

MiniAppBrowserContext::~MiniAppBrowserContext() {
    BrowserContext::NotifyWillBeDestroyed(this);
    ShutdownStoragePartitions();
    g_browser_context = nullptr;
}

MiniAppBrowserContext* MiniAppBrowserContext::GetDefault() {
    DCHECK(g_browser_context);
    return g_browser_context;
}
MiniAppBrowserContext* MiniAppBrowserContext::FromWebContents(
        content::WebContents* web_contents) {
  return static_cast<MiniAppBrowserContext*>(web_contents->GetBrowserContext());
}

void MiniAppBrowserContext::InitPath() {
    std::unique_ptr<base::Environment> env(base::Environment::Create());
    base::FilePath config_dir(
        base::nix::GetXDGDirectory(env.get(),
                                   base::nix::kXdgConfigHomeEnvVar,
                                   base::nix::kDotConfigDir));
    path_ = config_dir.Append("mini_app");
    content::BrowserContext::Initialize(this, path_);
}

std::unique_ptr<content::ZoomLevelDelegate> MiniAppBrowserContext::CreateZoomLevelDelegate(
    const base::FilePath& partition_path) {
    return nullptr;
}

base::FilePath MiniAppBrowserContext::GetPath() {
    return path_;
}

bool MiniAppBrowserContext::IsOffTheRecord() {
    return false;
}

content::ResourceContext* MiniAppBrowserContext::GetResourceContext() {
    return resource_context_.get();
}

content::DownloadManagerDelegate* MiniAppBrowserContext::GetDownloadManagerDelegate() {
    return nullptr;
}

content::BrowserPluginGuestManager* MiniAppBrowserContext::GetGuestManager() {
    return nullptr;
}

storage::SpecialStoragePolicy* MiniAppBrowserContext::GetSpecialStoragePolicy() {
    return nullptr;
}

content::PushMessagingService* MiniAppBrowserContext::GetPushMessagingService() {
    return nullptr;
}

content::StorageNotificationService* MiniAppBrowserContext::GetStorageNotificationService() {
    return nullptr;
}

content::SSLHostStateDelegate* MiniAppBrowserContext::GetSSLHostStateDelegate() {
    return nullptr;
}

content::PermissionControllerDelegate* MiniAppBrowserContext::GetPermissionControllerDelegate() {
    return nullptr;
}

content::ClientHintsControllerDelegate* MiniAppBrowserContext::GetClientHintsControllerDelegate() {
    return nullptr;
}

content::BackgroundFetchDelegate* MiniAppBrowserContext::GetBackgroundFetchDelegate() {
    return nullptr;
}

content::BackgroundSyncController* MiniAppBrowserContext::GetBackgroundSyncController() {
    return nullptr;
}

content::BrowsingDataRemoverDelegate* MiniAppBrowserContext::GetBrowsingDataRemoverDelegate() {
    return nullptr;
}

}   //namespace mini_app