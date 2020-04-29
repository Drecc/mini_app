#ifndef MINI_APP_APP_MINI_CONTENT_RENDERER_OBERVER_H_
#define MINI_APP_APP_MINI_CONTENT_RENDERER_OBERVER_H_

#include "content/public/renderer/render_frame_observer.h"
#include "content/public/renderer/render_frame.h"
#include "v8/include/v8.h"

namespace content {
class RenderProcessHost;
}

namespace mini_app {

class MiniAppRenderFrameObserver : public content::RenderFrameObserver {
public:
    MiniAppRenderFrameObserver(content::RenderFrame* render_frame);
    ~MiniAppRenderFrameObserver() override;

    //content::RenderFrameObserver
    void DidCreateScriptContext(v8::Local<v8::Context> context,
                                      int32_t world_id) override;
    void WillReleaseScriptContext(v8::Local<v8::Context> context,
                                        int32_t world_id) override;
    void OnDestruct() override;

private:

  v8::Persistent<v8::FunctionTemplate> require_native_template_;
  v8::Persistent<v8::Object> function_data_;
  v8::Global<v8::Context> v8_context_;

};

}  // namespace mini_app

#endif  // MINI_APP_APP_MINI_CONTENT_RENDERER_OBERVER_H_
