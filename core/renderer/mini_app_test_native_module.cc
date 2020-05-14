#include "mini_app/core/renderer/mini_app_test_native_module.h"

#include "base/logging.h"

namespace mini_app {

const char* test_native_module_name = "test";

void TestFunction(const v8::FunctionCallbackInfo<v8::Value>& info) {
    v8::Isolate* isolate = info.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    LOG(ERROR) << "I`M TEST";
    // v8::Local<v8::Context> context = isolate->GetCurrentContext();
    info.GetReturnValue().Set(v8::Boolean::New(isolate, true));
}

MiniAppTestNativeModule::MiniAppTestNativeModule() {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::ObjectTemplate> object_template = v8::ObjectTemplate::New(isolate);
    object_template->Set(v8::String::NewFromUtf8(isolate, "TestFunction").ToLocalChecked(), v8::FunctionTemplate::New(isolate, TestFunction));
    test_object_template_.Reset(isolate, object_template);
}

MiniAppTestNativeModule::~MiniAppTestNativeModule() {
    test_object_template_.Reset();
}

v8::Local<v8::Object> MiniAppTestNativeModule::NewInstance() {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::EscapableHandleScope handle_scope(isolate);
    v8::Local<v8::ObjectTemplate> object = v8::Local<v8::ObjectTemplate>::New(isolate, test_object_template_);
    return handle_scope.Escape(object->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
}

std::string MiniAppTestNativeModule::Name() {
    return test_native_module_name;
}



} // namespace mini_app