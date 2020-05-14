#ifndef MINI_APP_APP_MINI_TEST_NATIVE_MODULE_H_
#define MINI_APP_APP_MINI_TEST_NATIVE_MODULE_H_

#include "mini_app/core/renderer/mini_app_native_module.h"
#include "v8/include/v8.h"

namespace mini_app {

class MiniAppTestNativeModule : public MiniAppNativeModule {
public:
    explicit MiniAppTestNativeModule();
    ~MiniAppTestNativeModule() override;

    //MiniAppNativeModule
    v8::Handle<v8::Object> NewInstance() override;
    std::string Name() override;

private:
    v8::Persistent<v8::ObjectTemplate> test_object_template_;

};


}   //namespace mini_app

#endif //MINI_APP_APP_MINI_TEST_NATIVE_MODULE_H_