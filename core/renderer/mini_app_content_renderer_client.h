#ifndef MINI_APP_APP_MINI_CONTENT_RENDERER_CLIENT_H_
#define MINI_APP_APP_MINI_CONTENT_RENDERER_CLIENT_H_

#include "content/public/renderer/content_renderer_client.h"

namespace content {
class RenderProcessHost;
}

namespace mini_app {

class MiniAppContentRendererClient : public content::ContentRendererClient {
public:
    MiniAppContentRendererClient();
    ~MiniAppContentRendererClient() override;

    //content::ContentRendererClient
    void RenderFrameCreated(content::RenderFrame* render_frame) override;

private:

};

}  // namespace mini_app

#endif  // MINI_APP_APP_MINI_CONTENT_RENDERER_CLIENT_H_
