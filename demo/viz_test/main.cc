#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/logging.h"
#include "base/message_loop/message_loop.h"
#include "base/run_loop.h"
#include "base/task/thread_pool/thread_pool_instance.h"
#include "base/threading/thread.h"
#include "mini_app/demo/viz_test/demo_window.h"
#include "mojo/core/embedder/embedder.h"
#include "mojo/core/embedder/scoped_ipc_support.h"
#include "ui/events/platform/platform_event_source.h"

#if defined(USE_X11)
#include "ui/platform_window/x11/x11_window.h"
#endif

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

class InitUI {
 public:
  InitUI() {
#if defined(USE_X11)
    XInitThreads();
#endif
    event_source_ = ui::PlatformEventSource::CreateDefault();
  }

  ~InitUI() = default;

 private:
  std::unique_ptr<ui::PlatformEventSource> event_source_;

  DISALLOW_COPY_AND_ASSIGN(InitUI);
};

int RunMain() {
  base::RunLoop run_loop;

  mini_app::DemoWindow demo_window;
  demo_window.Create(gfx::Rect(800,600));

  run_loop.Run();
  return 0;
}

int main(int argc, char** argv) {
  MainEnv env(argc, argv);
  MoJoSupport mojo_support;
  InitUI init_ui;
  LOG(INFO) << "start viz test demo...";
  return RunMain();
}