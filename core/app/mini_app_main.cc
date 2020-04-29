#include "content/public/app/content_main.h"
#include "build/build_config.h"
#include "mini_app/core/app/mini_app_main_delegate.h"

#if defined(OS_WIN)
#include "content/public/app/sandbox_helper_win.h"
#include "sandbox/win/src/sandbox_types.h"

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE, wchar_t*, int) {
    mini_app::MiniAppMainDelegate delegate;
    content::ContentMainParams params(&delegate);
    sandbox::SandboxInterfaceInfo sandbox_info = {0};
    content::InitializeSandboxInfo(&sandbox_info);
    params.instance = instance;
    params.sandbox_info = &sandbox_info;
    return content::ContentMain(params);
}
#endif

#if defined(OS_LINUX)
int main(int argc, const char** argv) {
    mini_app::MiniAppMainDelegate delegate;
    content::ContentMainParams params(&delegate);
    params.argc = argc;
    params.argv = argv;
    return content::ContentMain(params);
}
#endif
