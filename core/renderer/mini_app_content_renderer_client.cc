#include "mini_app/core/renderer/mini_app_content_renderer_client.h"

#include "mini_app/core/renderer/mini_app_render_frame_observer.h"


namespace mini_app {

MiniAppContentRendererClient::MiniAppContentRendererClient() {
}

MiniAppContentRendererClient::~MiniAppContentRendererClient() {
}

void MiniAppContentRendererClient::RenderFrameCreated(content::RenderFrame* render_frame) {
    new MiniAppRenderFrameObserver(render_frame);
}

}  // namespace mini_app
