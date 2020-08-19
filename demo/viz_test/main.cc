#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/logging.h"
#include "base/message_loop/message_loop.h"
#include "base/run_loop.h"
#include "base/threading/thread.h"
#include "base/task/thread_pool/thread_pool_instance.h"
#include "mojo/core/embedder/embedder.h"
#include "mojo/core/embedder/scoped_ipc_support.h"

class MainEnv {
public:
  explicit MainEnv(int argc, char** argv) {
    base::CommandLine::Init(argc, argv);
    logging::SetLogItems(true, true, true, false);
    base::ThreadPoolInstance::CreateAndStartWithDefaultParams("viz_test");
  }
  ~MainEnv() = default;
private:
  base::AtExitManager at_exit_;
  base::MessageLoop message_loop_;
};

class MoJoSupport {
public:
  explicit MoJoSupport() : mojo_thread_("mojo") {
    mojo::core::Init();
    mojo_thread_.StartWithOptions(
        base::Thread::Options(base::MessagePumpType::IO, 0));
    ipc_support_ = std::make_unique<mojo::core::ScopedIPCSupport>(
        mojo_thread_.task_runner(),
        mojo::core::ScopedIPCSupport::ShutdownPolicy::CLEAN);
    
  }
  ~MoJoSupport() = default;
private:
  base::Thread mojo_thread_;
  std::unique_ptr<mojo::core::ScopedIPCSupport> ipc_support_;
};

int RunMain() {
  base::RunLoop run_loop;
  run_loop.Run();
  return 0;
}

int main(int argc, char** argv) {
  MainEnv env(argc, argv);
  MoJoSupport mojo_support;
  LOG(INFO) << "start viz test demo...";
  return RunMain();
}