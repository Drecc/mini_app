#include "mini_app/core/renderer/mini_app_render_frame_observer.h"

#include "mini_app/core/renderer/mini_app_module_system.h"
#include "mini_app/core/renderer/mini_app_test_native_module.h"

namespace mini_app {

MiniAppRenderFrameObserver::MiniAppRenderFrameObserver(content::RenderFrame* render_frame)
    : RenderFrameObserver(render_frame) {
    LOG(ERROR) << "create MiniAppRenderFrameObserver";
}

MiniAppRenderFrameObserver::~MiniAppRenderFrameObserver() {
    LOG(ERROR) << "delete MiniAppRenderFrameObserver";
}

void MiniAppRenderFrameObserver::DidCreateScriptContext(v8::Local<v8::Context> context, int32_t world_id) {
    LOG(ERROR) << "===DidCreateScriptContext";
    auto* module_system = new MiniAppModuleSystem(context);
    MiniAppModuleSystem::SetMiniAppModuleSystemInContext(module_system, context);

    auto* test = new MiniAppTestNativeModule();
    module_system->RegisterNativeModule(test->Name(), test);


    module_system->Init();
}

void MiniAppRenderFrameObserver::WillReleaseScriptContext(v8::Local<v8::Context> context, int32_t world_id) {
    LOG(ERROR) << "===WillReleaseScriptContext";
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handle_scope(isolate);
    require_native_template_.Reset();
    function_data_.Reset();
    v8_context_.Reset();

    MiniAppModuleSystem::ReleaseMiniAppModuleSystemInContext(context);

}

void MiniAppRenderFrameObserver::OnDestruct() {
    LOG(ERROR) << "delete OnDestruct";
    delete this;
}

}  // namespace mini_app
