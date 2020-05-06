#include "mini_app/core/renderer/mini_app_module_system.h"

#include "base/logging.h"

namespace mini_app {

const char* kMiniAppModuleSystem = "kMiniAppModuleSystem";
const char* kRequireNative = "requireNative";
const int kDataIndex = 10;

void RequireNativeFunction(const v8::FunctionCallbackInfo<v8::Value>& info) {
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


MiniAppModuleSystem::MiniAppModuleSystem(v8::Local<v8::Context> context) {
    v8::Isolate* isolate = context->GetIsolate();
    v8_context_.Reset(isolate, context);

    v8::HandleScope scope(isolate);
    v8::Local<v8::Object> function_data = v8::Object::New(isolate);
    function_data->Set(context, v8::String::NewFromUtf8(isolate, kMiniAppModuleSystem).ToLocalChecked(), v8::External::New(isolate, this)).Check();
    v8::Local<v8::FunctionTemplate> function_template = v8::FunctionTemplate::New(isolate, RequireNativeFunction, function_data);

    require_native_template_.Reset(isolate, function_template);
}

MiniAppModuleSystem::~MiniAppModuleSystem() {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handle_scope(isolate);
    v8_context_.Reset();
    function_data_.Reset();
    require_native_template_.Reset();
    LOG(ERROR) << "delete me";
}

void MiniAppModuleSystem::Init() {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> v8_context = GetV8Context();
    v8::Local<v8::Object> object = v8_context->Global();
    v8::Local<v8::String> part = v8::String::NewFromUtf8(isolate, kRequireNative).ToLocalChecked();
    v8::MaybeLocal<v8::Value> maybe_value = object->Get(v8_context, part);
    if(!maybe_value.IsEmpty() && maybe_value.ToLocalChecked()->IsUndefined()) {
        object->Set(v8_context, part, require_native_template_.Get(isolate)->GetFunction(v8_context).ToLocalChecked()).Check();
    }
}

v8::Local<v8::Context> MiniAppModuleSystem::GetV8Context() {
    return v8::Local<v8::Context>::New(v8::Isolate::GetCurrent(), v8_context_);
}

//static
void MiniAppModuleSystem::SetMiniAppModuleSystemInContext(MiniAppModuleSystem* module_system, v8::Local<v8::Context> context) {
    context->SetAlignedPointerInEmbedderData(kDataIndex, module_system);
}

//static
MiniAppModuleSystem* MiniAppModuleSystem::GetMiniAppModuleSystemInContext(v8::Local<v8::Context> context) {
    return reinterpret_cast<MiniAppModuleSystem*>(context->GetAlignedPointerFromEmbedderData(kDataIndex));
}

//static
void MiniAppModuleSystem::ReleaseMiniAppModuleSystemInContext(v8::Local<v8::Context> context) {
    delete GetMiniAppModuleSystemInContext(context);
    context->SetAlignedPointerInEmbedderData(kDataIndex, nullptr);
}

}   //namespace mini_app