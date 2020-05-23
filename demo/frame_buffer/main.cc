#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/logging.h"
#include "base/message_loop/message_loop.h"
#include "base/run_loop.h"
#include "mini_app/demo/frame_buffer/output_framebuffer.h"
 
int main(int argc, char **argv)
{
    base::AtExitManager at_exit;
    base::CommandLine::Init(argc, argv);
    base::MessageLoop message_loop;
    auto* frame_buffer_out = mini_app::OutputFrameBuffer::GetInstance();
    if(frame_buffer_out) {
        LOG(INFO) << frame_buffer_out->ToString();
    }
    base::RunLoop().Run();

    return 0;
}