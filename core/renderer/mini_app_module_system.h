#ifndef MINI_APP_APP_MINI_NATIVE_SYSTEM_H_
#define MINI_APP_APP_MINI_NATIVE_SYSTEM_H_

#include "v8/include/v8.h"
#include <map>


namespace mini_app {

class MiniAppNativeModule;

class  MiniAppModuleSystem {
public:
   explicit MiniAppModuleSystem(v8::Local<v8::Context> context);
    ~MiniAppModuleSystem();

    static void SetMiniAppModuleSystemInContext(MiniAppModuleSystem* module_system, v8::Local<v8::Context> context);
    static MiniAppModuleSystem* GetMiniAppModuleSystemInContext(v8::Local<v8::Context> context);
    static void ReleaseMiniAppModuleSystemInContext(v8::Local<v8::Context> context);

    void Init();
    void RegisterNativeModule(const std::string& name, MiniAppNativeModule* native_module);
    v8::Local<v8::Object> RequireNative(const std::string& name);

private:
    v8::Local<v8::Context> GetV8Context();
    
    v8::Persistent<v8::FunctionTemplate> require_native_template_;
    v8::Persistent<v8::Context> v8_context_;
    v8::Persistent<v8::Object> function_data_;
    std::map<std::string, MiniAppNativeModule*> native_module_map_;
};

}  // namespace mini_app

#endif  // MINI_APP_APP_MINI_NATIVE_SYSTEM_H_
