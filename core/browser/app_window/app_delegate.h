#ifndef MINI_APP_CORE_BROWSER_APP_WINDOW_APP_DELEGATE_H_
#define MINI_APP_CORE_BROWSER_APP_WINDOW_APP_DELEGATE_H_

#include "base/callback_forward.h"
#include "content/public/browser/media_stream_request.h"
#include "third_party/blink/public/common/mediastream/media_stream_request.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/base/window_open_disposition.h"

namespace blink {
namespace mojom {
class FileChooserParams;
}
}  // namespace blink

namespace content {
enum class PictureInPictureResult;
class BrowserContext;
class ColorChooser;
class FileSelectListener;
class RenderFrameHost;
class RenderViewHost;
class WebContents;
struct OpenURLParams;
}

namespace gfx {
class Rect;
class Size;
}

namespace viz {
class SurfaceId;
}

namespace mini_app {

namespace app_window {

// Interface to give packaged apps access to services in the browser, for things
// like handling links and showing UI prompts to the user.
class AppDelegate {
 public:
  virtual ~AppDelegate() {}

  // General initialization.
  virtual void InitWebContents(content::WebContents* web_contents) = 0;
  virtual void RenderViewCreated(content::RenderViewHost* render_view_host) = 0;

  // Resizes WebContents.
  virtual void ResizeWebContents(content::WebContents* web_contents,
                                 const gfx::Size& size) = 0;

  // Link handling.
  virtual content::WebContents* OpenURLFromTab(
      content::BrowserContext* context,
      content::WebContents* source,
      const content::OpenURLParams& params) = 0;
  virtual void AddNewContents(
      content::BrowserContext* context,
      std::unique_ptr<content::WebContents> new_contents,
      WindowOpenDisposition disposition,
      const gfx::Rect& initial_rect,
      bool user_gesture) = 0;

  // Feature support.
  virtual content::ColorChooser* ShowColorChooser(
      content::WebContents* web_contents,
      SkColor initial_color) = 0;
  virtual void RunFileChooser(
      content::RenderFrameHost* render_frame_host,
      std::unique_ptr<content::FileSelectListener> listener,
      const blink::mojom::FileChooserParams& params) = 0;
  virtual int PreferredIconSize() const = 0;

  // Web contents modal dialog support.
  virtual void SetWebContentsBlocked(content::WebContents* web_contents,
                                     bool blocked) = 0;
  virtual bool IsWebContentsVisible(content::WebContents* web_contents) = 0;

  // |callback| will be called when the process is about to terminate.
  virtual void SetTerminatingCallback(const base::Closure& callback) = 0;

  // Called when the app is hidden or shown.
  virtual void OnHide() = 0;
  virtual void OnShow() = 0;

  // Called when app web contents finishes focus traversal - gives the delegate
  // a chance to handle the focus change.
  // Return whether focus has been handled.
  virtual bool TakeFocus(content::WebContents* web_contents, bool reverse) = 0;

  // Notifies the Picture-in-Picture controller that there is a new player
  // entering Picture-in-Picture.
  // Returns the result of the enter request.
  virtual content::PictureInPictureResult EnterPictureInPicture(
      content::WebContents* web_contents,
      const viz::SurfaceId& surface_id,
      const gfx::Size& natural_size) = 0;

  // Updates the Picture-in-Picture controller with a signal that
  // Picture-in-Picture mode has ended.
  virtual void ExitPictureInPicture() = 0;
};

}  // namespace app_window

}   // namespace mini_app

#endif  // MINI_APP_CORE_BROWSER_APP_WINDOW_APP_DELEGATE_H_
