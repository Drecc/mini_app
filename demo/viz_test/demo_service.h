#ifndef MINI_APP_DEMO_VIZ_TEST_DEMO_SERVICE_H_
#define MINI_APP_DEMO_VIZ_TEST_DEMO_SERVICE_H_

#include <memory>

#include "base/threading/thread.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "services/viz/privileged/mojom/compositing/frame_sink_manager.mojom.h"

namespace viz {
class VizCompositorThreadRunnerImpl;
}  // namespace viz

namespace mini_app {

class DemoService {
 public:
  DemoService(mojo::PendingReceiver<viz::mojom::FrameSinkManager> receiver,
              mojo::PendingRemote<viz::mojom::FrameSinkManagerClient> client);
  ~DemoService();

 private:
  std::unique_ptr<viz::VizCompositorThreadRunnerImpl> runner_;

  DISALLOW_COPY_AND_ASSIGN(DemoService);
};

} // namespace mini_app

#endif //MINI_APP_DEMO_VIZ_TEST_DEMO_SERVICE_H_