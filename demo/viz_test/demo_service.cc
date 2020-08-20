#include "mini_app/demo/viz_test/demo_service.h"

#include <memory>
#include <utility>

#include "components/viz/common/frame_sinks/begin_frame_source.h"
#include "components/viz/service/main/viz_compositor_thread_runner_impl.h"

namespace mini_app {

DemoService::DemoService(
    mojo::PendingReceiver<viz::mojom::FrameSinkManager> receiver,
    mojo::PendingRemote<viz::mojom::FrameSinkManagerClient> client) {
  auto params = viz::mojom::FrameSinkManagerParams::New();
  params->restart_id = viz::BeginFrameSource::kNotRestartableId;
  params->use_activation_deadline = false;
  params->activation_deadline_in_frames = 0u;
  params->frame_sink_manager = std::move(receiver);
  params->frame_sink_manager_client = std::move(client);
  runner_ = std::make_unique<viz::VizCompositorThreadRunnerImpl>();
  runner_->CreateFrameSinkManager(std::move(params));
}

DemoService::~DemoService() = default;
  
}