#include "base/logging.h"
#include "base/command_line.h"
#include "base/at_exit.h"
#include "base/message_loop/message_loop.h"
#include "base/run_loop.h"
#include "mini_app/demo/frame_buffer/output_framebuffer.h"

int main(int argc, char** argv) {
    base::AtExitManager exit;
    base::CommandLine::Init(argc, argv);
    base::MessageLoop message_loop;

    LOG(INFO) << "start mini_app ...";
    mini_app::OutputFrameBuffer frame_buffer;
    if(!frame_buffer.Init(1280, 720)) {
        return -1;
    }
    base::RunLoop().Run();

    return 0;
}