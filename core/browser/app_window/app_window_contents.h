#ifndef MINI_APP_CORE_BROWSER_APP_WINDOW_APP_WINDOW_CONTENTS_H_
#define MINI_APP_CORE_BROWSER_APP_WINDOW_APP_WINDOW_CONTENTS_H_

#include <stdint.h>

#include <memory>

#include "base/macros.h"
#include "content/public/browser/web_contents_observer.h"
#include "mini_app/core/browser/app_window/app_window.h"
#include "url/gurl.h"

namespace content {
class BrowserContext;
class RenderFrameHost;
}

namespace mini_app {

namespace app_window {

class AppWindowContents : public content::WebContentsObserver {
public:
  AppWindowContents(AppWindow* host);
  ~AppWindowContents() override;

  virtual void Initialize(content::BrowserContext* context,
                          const GURL& url);
  virtual void LoadContents(int32_t creator_process_id);
  virtual void NativeWindowChanged(NativeAppWindow* native_app_window);
  virtual void NativeWindowClosed(bool send_onclosed);

  virtual content::WebContents* GetWebContents() const;

  // content::WebContentsObserver
  bool OnMessageReceived(const IPC::Message& message,
                         content::RenderFrameHost* sender) override;
  void DidFinishNavigation(content::NavigationHandle* handle) override;

private:
  AppWindow* host_;
  GURL url_;
  std::unique_ptr<content::WebContents> web_contents_;

  DISALLOW_COPY_AND_ASSIGN(AppWindowContents);
};

}  // namespace app_window

}   // namespace mini_app

#endif  // MINI_APP_CORE_BROWSER_APP_WINDOW_APP_WINDOW_CONTENTS_H_
