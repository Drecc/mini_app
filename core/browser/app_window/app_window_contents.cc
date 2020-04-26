#include "mini_app/core/browser/app_window/app_window_contents.h"

#include <memory>
#include <string>
#include <utility>

#include "content/public/browser/browser_context.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/navigation_handle.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/render_process_host.h"
#include "content/public/browser/render_view_host.h"
#include "content/public/browser/site_instance.h"
#include "content/public/browser/web_contents.h"
#include "mini_app/core/browser/app_window/native_app_window.h"

namespace mini_app {

namespace app_window {

AppWindowContents::AppWindowContents(AppWindow* host)
     : host_(host) {
      
}

AppWindowContents::~AppWindowContents() {
}

void AppWindowContents::Initialize(content::BrowserContext* context,
                                       const GURL& url) {
  url_ = url;
  auto site = content::SiteInstance::CreateForURL(context, url);
  content::WebContents::CreateParams create_params(
      context, site);
  web_contents_ = content::WebContents::Create(create_params);

  Observe(web_contents_.get());
}

void AppWindowContents::LoadContents(int32_t creator_process_id) {
  // Sandboxed page that are not in the Chrome App package are loaded in a
  // different process.
  if (web_contents_->GetMainFrame()->GetProcess()->GetID() !=
      creator_process_id) {
    VLOG(1) << "AppWindow created in new process ("
            << web_contents_->GetMainFrame()->GetProcess()->GetID()
            << ") != creator (" << creator_process_id << "). Routing disabled.";
  }
  web_contents_->GetController().LoadURL(
      url_, content::Referrer(), ui::PAGE_TRANSITION_LINK,
      std::string());
}

void AppWindowContents::NativeWindowChanged(
    NativeAppWindow* native_app_window) {
}

void AppWindowContents::NativeWindowClosed(bool send_onclosed) {
}

content::WebContents* AppWindowContents::GetWebContents() const {
  return web_contents_.get();
}

bool AppWindowContents::OnMessageReceived(
    const IPC::Message& message,
    content::RenderFrameHost* sender) {
  return false;
}

void AppWindowContents::DidFinishNavigation(
    content::NavigationHandle* handle) {
  host_->OnDidFinishFirstNavigation();
}

}  // namespace app_window

} // namespace mini_app
