#include "content/public/app/content_main.h"

#include "mini_app/core/app/mini_app_main_delegate.h"

int main(int argc, const char** argv) {
    mini_app::MiniAppMainDelegate delegate;
    content::ContentMainParams params(&delegate);
    params.argc = argc;
    params.argv = argv;
    return content::ContentMain(params);
}
