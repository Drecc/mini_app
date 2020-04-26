#ifndef MINI_APP_APP_MINI_APP_BROWSER_CONTEXT_H_
#define MINI_APP_APP_MINI_APP_BROWSER_CONTEXT_H_

#include "base/files/file_path.h"
#include "content/public/browser/browser_context.h"

namespace content {
class WebContents;
}

namespace mini_app {
class MiniAppResourceContext;

class MiniAppBrowserContext : public content::BrowserContext {
public:
    MiniAppBrowserContext();
    ~MiniAppBrowserContext() override;
    
    static MiniAppBrowserContext* GetDefault();
    static MiniAppBrowserContext* FromWebContents(
        content::WebContents* web_contents);

    //content::BrowserContext
    std::unique_ptr<content::ZoomLevelDelegate> CreateZoomLevelDelegate(
        const base::FilePath& partition_path) override;
    base::FilePath GetPath() override;
    bool IsOffTheRecord() override;
    content::ResourceContext* GetResourceContext() override;
    content::DownloadManagerDelegate* GetDownloadManagerDelegate() override;
    content::BrowserPluginGuestManager* GetGuestManager() override;
    storage::SpecialStoragePolicy* GetSpecialStoragePolicy() override;
    content::PushMessagingService* GetPushMessagingService() override;
    content::StorageNotificationService* GetStorageNotificationService() override;
    content::SSLHostStateDelegate* GetSSLHostStateDelegate() override;
    content::PermissionControllerDelegate* GetPermissionControllerDelegate() override;
    content::ClientHintsControllerDelegate* GetClientHintsControllerDelegate() override;
    content::BackgroundFetchDelegate* GetBackgroundFetchDelegate() override;
    content::BackgroundSyncController* GetBackgroundSyncController() override;
    content::BrowsingDataRemoverDelegate* GetBrowsingDataRemoverDelegate() override;

private:
    void InitPath();
    
    base::FilePath path_;
    std::unique_ptr<MiniAppResourceContext> resource_context_;

    DISALLOW_COPY_AND_ASSIGN(MiniAppBrowserContext);
};

}  //namespace mini_app


#endif //  MINI_APP_APP_MINI_APP_BROWSER_CONTEXT_H_