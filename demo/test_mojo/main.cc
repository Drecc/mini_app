#include "base/command_line.h"
#include "base/logging.h"
#include "base/message_loop/message_loop.h"
#include "base/run_loop.h"
#include "base/threading/thread.h"
#include "base/task/thread_pool/thread_pool_instance.h"
#include "base/process/launch.h"

#include "mojo/core/embedder/embedder.h"
#include "mojo/core/embedder/scoped_ipc_support.h"
#include "mojo/public/cpp/platform/platform_channel.h"
#include "mojo/public/cpp/system/invitation.h"
#include "mojo/public/cpp/system/message_pipe.h"
#include "mojo/public/cpp/system/message.h"
#include "mojo/public/cpp/system/wait.h"

//mojo/public/cpp/system/README.md
//mojo/public/cpp/bindings/README.md

mojo::ScopedMessagePipeHandle Launch() {
  mojo::PlatformChannel platform_channal;
  mojo::OutgoingInvitation outgoing_invitation;

  auto scope_handle = outgoing_invitation.AttachMessagePipe("test");
  base::LaunchOptions option;
  base::CommandLine command_line(base::CommandLine::ForCurrentProcess()->GetProgram());
  platform_channal.PrepareToPassRemoteEndpoint(&option, &command_line);
  base::Process process = base::LaunchProcess(command_line, option);
  platform_channal.RemoteProcessLaunchAttempted();

  mojo::OutgoingInvitation::Send(std::move(outgoing_invitation), process.Handle(),
                          platform_channal.TakeLocalEndpoint());
  return scope_handle;
}

mojo::ScopedMessagePipeHandle Accept() {
  mojo::IncomingInvitation incoming_invitation = mojo::IncomingInvitation::Accept(
    mojo::PlatformChannel::RecoverPassedEndpointFromCommandLine(*base::CommandLine::ForCurrentProcess()));
  auto scope_handle = incoming_invitation.ExtractMessagePipe("test");
  return scope_handle;
}


int main(int argc, char** argv) {
  base::CommandLine::Init(argc, argv);

  //init mojo
  mojo::core::Init();
  base::Thread ipc_thread("ipc");
  ipc_thread.StartWithOptions(base::Thread::Options(base::MessagePumpType::IO, 0));
  mojo::core::ScopedIPCSupport ipc_support(
      ipc_thread.task_runner(),
      mojo::core::ScopedIPCSupport::ShutdownPolicy::CLEAN);

  mojo::ScopedMessagePipeHandle pipe_handle;
  std::string message;

  if(base::CommandLine::ForCurrentProcess()->HasSwitch(mojo::PlatformChannel::kHandleSwitch)) {
    logging::SetLogPrefix("render");
    pipe_handle = Accept();
    message = "hello, i`m render";
  }
  else {
    logging::SetLogPrefix("browser");
    pipe_handle = Launch();
    message = "hello, i`m browser";
  }

  LOG(INFO) << base::CommandLine::ForCurrentProcess()->GetCommandLineString();

  {
    {
      auto res = mojo::WriteMessageRaw(pipe_handle.get(), message.c_str(), message.size(), nullptr, 0, MOJO_WRITE_MESSAGE_FLAG_NONE);
      DCHECK_EQ(res, MOJO_RESULT_OK);
    }
    {
      MojoResult result = mojo::Wait(pipe_handle.get(), MOJO_HANDLE_SIGNAL_READABLE);
      DCHECK_EQ(result, MOJO_RESULT_OK);
      std::vector<uint8_t> data;
      auto res = mojo::ReadMessageRaw(pipe_handle.get(), &data, nullptr, MOJO_READ_MESSAGE_FLAG_NONE);
      DCHECK_EQ(res, MOJO_RESULT_OK);
      LOG(INFO) << "get message: " << (char*)&data[0];
    }
  }

  base::MessageLoop message_loop;
  base::RunLoop run_loop;
  run_loop.Run();

  return 0;
}