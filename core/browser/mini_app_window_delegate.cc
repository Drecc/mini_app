#include "mini_app/core/browser/mini_app_window_delegate.h"

#include "content/public/browser/file_select_listener.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_delegate.h"

namespace mini_app {

MiniAppDelegate::MiniAppDelegate() {
}

MiniAppDelegate::~MiniAppDelegate() {
}

void MiniAppDelegate::InitWebContents(content::WebContents* web_contents) {
}

void MiniAppDelegate::RenderViewCreated(
    content::RenderViewHost* render_view_host) {
  // The views implementation of AppWindow takes focus via SetInitialFocus()
  // and views::WebView but app_shell is aura-only and must do it manually.
  content::WebContents::FromRenderViewHost(render_view_host)->Focus();
}

void MiniAppDelegate::ResizeWebContents(content::WebContents* web_contents,
                                         const gfx::Size& size) {
  NOTIMPLEMENTED();
}

content::WebContents* MiniAppDelegate::OpenURLFromTab(
    content::BrowserContext* context,
    content::WebContents* source,
    const content::OpenURLParams& params) {
  NOTIMPLEMENTED();
  return NULL;
}

void MiniAppDelegate::AddNewContents(
    content::BrowserContext* context,
    std::unique_ptr<content::WebContents> new_contents,
    WindowOpenDisposition disposition,
    const gfx::Rect& initial_rect,
    bool user_gesture) {
  NOTIMPLEMENTED();
}

content::ColorChooser* MiniAppDelegate::ShowColorChooser(
    content::WebContents* web_contents,
    SkColor initial_color) {
  NOTIMPLEMENTED();
  return NULL;
}

void MiniAppDelegate::RunFileChooser(
    content::RenderFrameHost* render_frame_host,
    std::unique_ptr<content::FileSelectListener> listener,
    const blink::mojom::FileChooserParams& params) {
  NOTIMPLEMENTED();
  listener->FileSelectionCanceled();
}

int MiniAppDelegate::PreferredIconSize() const {
  return 32;
}

void MiniAppDelegate::SetWebContentsBlocked(
    content::WebContents* web_contents,
    bool blocked) {
  NOTIMPLEMENTED();
}

bool MiniAppDelegate::IsWebContentsVisible(
    content::WebContents* web_contents) {
  return true;
}

void MiniAppDelegate::SetTerminatingCallback(const base::Closure& callback) {
  // TODO(jamescook): Should app_shell continue to close the app window
  // manually or should it use a browser termination callback like Chrome?
}

bool MiniAppDelegate::TakeFocus(content::WebContents* web_contents,
                                 bool reverse) {
  return false;
}

content::PictureInPictureResult MiniAppDelegate::EnterPictureInPicture(
    content::WebContents* web_contents,
    const viz::SurfaceId& surface_id,
    const gfx::Size& natural_size) {
  NOTREACHED();
  return content::PictureInPictureResult::kNotSupported;
}

void MiniAppDelegate::ExitPictureInPicture() {
  NOTREACHED();
}

}  // namespace mini_app
