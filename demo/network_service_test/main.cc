#include "base/at_exit.h"
#include "base/bind.h"
#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/files/file.h"
#include "base/logging.h"
#include "base/macros.h"
#include "base/message_loop/message_loop.h"
#include "base/run_loop.h"
#include "base/task/post_task.h"
#include "base/task/thread_pool/thread_pool_instance.h"
#include "base/threading/thread.h"

//mojo
#include "mojo/core/embedder/embedder.h"
#include "mojo/core/embedder/scoped_ipc_support.h"
#include "mojo/public/cpp/bindings/remote.h"

//network service
#include "services/network/network_service.h"
#include "services/network/public/cpp/url_loader_completion_status.h"
#include "services/network/public/mojom/network_context.mojom.h"
#include "services/network/public/mojom/network_service.mojom.h"
#include "services/network/public/mojom/url_loader_factory.mojom.h"
#include "services/network/public/mojom/url_loader.mojom.h"
#include "services/network/public/mojom/url_response_head.mojom-forward.h"

class MainEnv {
public:
  explicit MainEnv(int argc, char** argv) {
    base::CommandLine::Init(argc, argv);
    logging::SetLogItems(true, true, true, false);
    base::ThreadPoolInstance::CreateAndStartWithDefaultParams("network_service_test");
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

class NetWorkService {
public:
  NetWorkService() : thread_("NetWorkService") {
    base::Thread::Options options(base::MessagePumpType::IO, 0);
    thread_.StartWithOptions(options);
  }
  ~NetWorkService() = default;

  void Init(mojo::PendingReceiver<network::mojom::NetworkService> receiver) {
    thread_.task_runner()->PostTask(FROM_HERE, base::BindOnce(&NetWorkService::CreateNetWorkService, base::Unretained(this), std::move(receiver)));
  }

private:
  void CreateNetWorkService(mojo::PendingReceiver<network::mojom::NetworkService> receiver) {
    if(!network_service_)
      network_service_ = network::NetworkService::Create(std::move(receiver));
  }
  std::unique_ptr<network::NetworkService> network_service_;
  base::Thread thread_;
};

class DemoUrlLoaderClient : public network::mojom::URLLoaderClient {
public:
  void OnReceiveResponse(network::mojom::URLResponseHeadPtr response_head) override {
    LOG(INFO) << "OnReceiveResponse";
  }

  void OnReceiveRedirect(const net::RedirectInfo& redirect_info,
                         network::mojom::URLResponseHeadPtr response_head) override {
    LOG(INFO) << "OnReceiveRedirect " << redirect_info.new_url;
  }

  void OnReceiveCachedMetadata(mojo_base::BigBuffer data) override {
    LOG(INFO) << "OnReceiveCachedMetadata";
  }

  void OnTransferSizeUpdated(int32_t transfer_size_diff) override {
    LOG(INFO) << "OnTransferSizeUpdated";
  }

  void OnUploadProgress(int64_t current_position,
                        int64_t total_size,
                        OnUploadProgressCallback ack_callback) override {
    LOG(INFO) << "OnUploadProgress";
  }

  void OnStartLoadingResponseBody(
      mojo::ScopedDataPipeConsumerHandle body) override {
    LOG(INFO) << "OnStartLoadingResponseBody";
  }

  void OnComplete(const network::URLLoaderCompletionStatus& status) override {
    LOG(INFO) << "OnComplete";
  }

  mojo::PendingRemote<network::mojom::URLLoaderClient> CreateRemote() {
    mojo::PendingRemote<network::mojom::URLLoaderClient> client_remote;
    receiver_.Bind(client_remote.InitWithNewPipeAndPassReceiver());
    return client_remote;
  }

private:
  mojo::Receiver<network::mojom::URLLoaderClient> receiver_{this};
};

int RunMain() {
  base::RunLoop run_loop;

  //创建NetworkService的Remote端
  mojo::Remote<network::mojom::NetworkService> remote_service_;

  //将NetworkService的Receiver端传递给NetworkService的实现
  NetWorkService service;
  service.Init(remote_service_.BindNewPipeAndPassReceiver());

  //查询与NetworkService是否连接建立成功
  remote_service_.QueryVersion(base::BindRepeating(
      [](uint32_t value) {
        LOG(INFO) << "create NetworkService success ";
      }));

  //log
  base::FilePath log_path("log_network");
  base::File file(log_path, base::File::FLAG_CREATE_ALWAYS | base::File::FLAG_WRITE);
  if (!file.IsValid()) {
    LOG(ERROR) << "Failed opening NetLog: " << log_path.value();
  } else {
    remote_service_->StartNetLog(std::move(file), net::NetLogCaptureMode::kEverything, base::DictionaryValue());
  }

  //创建一个NetworkContext的Remote端
  mojo::Remote<network::mojom::NetworkContext> network_context_;
  network::mojom::NetworkContextParamsPtr context_params =
        network::mojom::NetworkContextParams::New();
  //使用NetworkService的Remote端创建NetworkContext，并把NetworkContext的Receiver端传递给它
  remote_service_->CreateNetworkContext(
        network_context_.BindNewPipeAndPassReceiver(),
        std::move(context_params));

  //创建UrlLoaderClient的Recevier端
  std::unique_ptr<DemoUrlLoaderClient> client_;
  client_.reset(new DemoUrlLoaderClient());

  //创建URLLoader的Remote端
  mojo::Remote<network::mojom::URLLoader> loader_;
  loader_.reset();

  //创建URLLoaderFactory的Remote端
  mojo::Remote<network::mojom::URLLoaderFactory> loader_factory;
  network::mojom::URLLoaderFactoryParamsPtr params =
      network::mojom::URLLoaderFactoryParams::New();
  params->process_id = 0;
  params->is_corb_enabled = false;
  //使用NetworkContext的Remote端创建URLLoaderFactory，并把URLLoaderFactory的Receiver端传递给它
  network_context_->CreateURLLoaderFactory(
      loader_factory.BindNewPipeAndPassReceiver(), std::move(params));

  //创建一个ResourceRequest，它作为URLLoader请求的参数
  network::ResourceRequest request;
  request.url = GURL("http://www.google.com");
  request.method = "GET";
  request.request_initiator = url::Origin();
  //使用URLLoaderFactory的Remote端，创建URLLoader，并把URLLoader的Recevier端和UrlLoaderClient的Remote端传递给它，让它们建立起对应的关系，之后就会开始url请求
  loader_factory->CreateLoaderAndStart(
      loader_.BindNewPipeAndPassReceiver(), 1, 1, network::mojom::kURLLoadOptionNone, request,
      client_->CreateRemote(),
      net::MutableNetworkTrafficAnnotationTag(net::DefineNetworkTrafficAnnotation("test", "Traffic annotation")));

  run_loop.Run();
  return 0;
}

int main(int argc, char** argv) {
  MainEnv env(argc, argv);
  MoJoSupport mojo_support;
  LOG(INFO) << "start network service test demo...";
  return RunMain();
}

