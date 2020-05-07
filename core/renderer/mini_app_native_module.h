#ifndef MINI_APP_APP_MINI_NATIVE_MODULE_H_
#define MINI_APP_APP_MINI_NATIVE_MODULE_H_

#include "v8/include/v8.h"

namespace mini_app {

class  MiniAppNativeModule {
public:
    virtual v8::Handle<v8::Object> NewInstance() = 0;
    virtual ~MiniAppNativeModule() {}
};

}  // namespace mini_app

#endif  // MINI_APP_APP_MINI_NATIVE_MODULE_H_
