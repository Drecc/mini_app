#include "mini_app/core/renderer/mini_app_render_frame_observer.h"


namespace mini_app {

MiniAppRenderFrameObserver::MiniAppRenderFrameObserver(content::RenderFrame* render_frame)
    : RenderFrameObserver(render_frame) {
    LOG(ERROR) << "create MiniAppRenderFrameObserver";
}

MiniAppRenderFrameObserver::~MiniAppRenderFrameObserver() {
    LOG(ERROR) << "delete MiniAppRenderFrameObserver";
}

void LOGFromJS(const v8::FunctionCallbackInfo<v8::Value>& info) {
    v8::ReturnValue<v8::Value> result(info.GetReturnValue());
    if (info.Length() < 1) {
        result.SetUndefined();
        return;
    }
    v8::Local<v8::Value> arg = info[0];
    v8::String::Utf8Value value(info.GetIsolate(), arg);
    LOG(ERROR) << "log from js:  " << *value;
    result.Set(v8::Boolean::New(info.GetIsolate(), true));
}

void MiniAppRenderFrameObserver::DidCreateScriptContext(v8::Local<v8::Context> context, int32_t world_id) {
    LOG(ERROR) << "===DidCreateScriptContext";
    v8::Isolate* isolate = context->GetIsolate();
    // v8_context_.Reset(isolate, context);

    v8::HandleScope handle_scope(isolate);
    v8::Handle<v8::Object> function_data = context->Global();
    v8::Handle<v8::FunctionTemplate> require_native_template = v8::FunctionTemplate::New(isolate, LOGFromJS);

    v8::Handle<v8::Function> require_native = require_native_template->GetFunction(context).ToLocalChecked();
    function_data->Set(context, v8::String::NewFromUtf8(isolate, "LOG").ToLocalChecked(), require_native).Check();

    // function_data_.Reset(isolate, function_data);
    // require_native_template_.Reset(isolate, require_native_template);
}

void MiniAppRenderFrameObserver::WillReleaseScriptContext(v8::Local<v8::Context> context, int32_t world_id) {
    LOG(ERROR) << "===WillReleaseScriptContext";
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope handle_scope(isolate);
  require_native_template_.Reset();
  function_data_.Reset();
  v8_context_.Reset();                                
}

void MiniAppRenderFrameObserver::OnDestruct() {
    LOG(ERROR) << "delete OnDestruct";
    delete this;
}

}  // namespace mini_app
