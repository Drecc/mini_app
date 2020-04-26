#ifndef MINI_APP_CORE_BROWSER_UI_APPS_CHROME_APP_DELEGATE_H_
#define MINI_APP_CORE_BROWSER_UI_APPS_CHROME_APP_DELEGATE_H_

#include <memory>

#include "base/callback.h"
#include "base/macros.h"
#include "base/memory/weak_ptr.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "mini_app/core/browser/app_window/app_delegate.h"
#include "ui/base/window_open_disposition.h"
#include "ui/gfx/geometry/rect.h"

namespace mini_app {

class MiniAppDelegate : public app_window::AppDelegate {
 public:
  explicit MiniAppDelegate();
  ~MiniAppDelegate() override;

  // app_window::AppDelegate
  void InitWebContents(content::WebContents* web_contents) override;
  void RenderViewCreated(content::RenderViewHost* render_view_host) override;
  void ResizeWebContents(content::WebContents* web_contents,
                         const gfx::Size& size) override;
  content::WebContents* OpenURLFromTab(
      content::BrowserContext* context,
      content::WebContents* source,
      const content::OpenURLParams& params) override;
  void AddNewContents(content::BrowserContext* context,
                      std::unique_ptr<content::WebContents> new_contents,
                      WindowOpenDisposition disposition,
                      const gfx::Rect& initial_rect,
                      bool user_gesture) override;
  content::ColorChooser* ShowColorChooser(content::WebContents* web_contents,
                                          SkColor initial_color) override;
  void RunFileChooser(content::RenderFrameHost* render_frame_host,
                      std::unique_ptr<content::FileSelectListener> listener,
                      const blink::mojom::FileChooserParams& params) override;
  int PreferredIconSize() const override;
  void SetWebContentsBlocked(content::WebContents* web_contents,
                             bool blocked) override;
  bool IsWebContentsVisible(content::WebContents* web_contents) override;
  void SetTerminatingCallback(const base::Closure& callback) override;
  void OnHide() override {}
  void OnShow() override {}
  bool TakeFocus(content::WebContents* web_contents, bool reverse) override;
  content::PictureInPictureResult EnterPictureInPicture(
      content::WebContents* web_contents,
      const viz::SurfaceId& surface_id,
      const gfx::Size& natural_size) override;
  void ExitPictureInPicture() override;

 private:

  DISALLOW_COPY_AND_ASSIGN(MiniAppDelegate);
};

}

#endif  // MINI_APP_CORE_BROWSER_UI_APPS_CHROME_APP_DELEGATE_H_
