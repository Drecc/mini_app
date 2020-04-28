#include "mini_app/core/application/browser/application_url_loader_factory.h"

#include "base/task/post_task.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/file_url_loader.h"
#include "content/public/browser/web_contents.h"
#include "mini_app/core/application/browser/application_service.h"
#include "mini_app/core/application/common/application_file_util.h"
#include "mini_app/core/common/mini_app_scoped_allow_blocking.h"
#include "net/base/filename_util.h"
#include "net/base/mime_util.h"
#include "net/base/net_errors.h"
#include "services/network/public/cpp/resource_request.h"
#include "services/network/public/mojom/url_loader.mojom.h"
#include "services/network/public/mojom/url_response_head.mojom.h"

namespace mini_app {

namespace application {


ApplicationUrlLoaderFactory::ApplicationUrlLoaderFactory(content::BrowserContext* browser_context, content::WebContents* web_contents)
     {

}

ApplicationUrlLoaderFactory::~ApplicationUrlLoaderFactory() {
    
}

void ApplicationUrlLoaderFactory::CreateLoaderAndStart(
        mojo::PendingReceiver<::network::mojom::URLLoader> loader, 
        int32_t routing_id, 
        int32_t request_id, 
        uint32_t options, 
        const ::network::ResourceRequest& request, 
        mojo::PendingRemote<::network::mojom::URLLoaderClient> client, 
        const ::net::MutableNetworkTrafficAnnotationTag& traffic_annotation) {
    Application* app = ApplicationService::GetInstance()->GetApplicationByID(request.url.host());
    if(app) {
        base::FilePath relative_path =
                ApplicationURLToRelativeFilePath(request.url);
        base::FilePath directory_path = app->data()->path();
        ApplicationResource resource(app->id(), directory_path, relative_path);
        base::FilePath* file_path = new base::FilePath;

        base::PostTaskAndReply(
            FROM_HERE, {base::ThreadPool(), base::MayBlock()},
            base::BindOnce(&GetFilePath, resource, base::Unretained(file_path)),
            base::BindOnce(
                &OnGetFilePath, base::Owned(file_path), request, std::move(loader),
                std::move(client)));
        return;

    }
    mojo::Remote<network::mojom::URLLoaderClient>(std::move(client))
            ->OnComplete(network::URLLoaderCompletionStatus(net::ERR_FAILED));
}

void ApplicationUrlLoaderFactory::GetFilePath(const ApplicationResource& resource, base::FilePath* file_path) {
    //迫于无奈 必须在线程池里的mayblock下，才能进行io操作
    *file_path = resource.GetFilePath();
}

void ApplicationUrlLoaderFactory::OnGetFilePath(
        base::FilePath* file_path, 
        network::ResourceRequest request,
        mojo::PendingReceiver<network::mojom::URLLoader> loader,
        mojo::PendingRemote<network::mojom::URLLoaderClient> client) {
    request.url = net::FilePathToFileURL(*file_path);
    base::PostTask(
        FROM_HERE, {content::BrowserThread::IO},
        base::BindOnce(
            &StartFileJob, 
            std::move(request), 
            std::move(loader),
            std::move(client),
            BuildHttpHeaders()));
}

void ApplicationUrlLoaderFactory::StartFileJob(
        network::ResourceRequest request,
        mojo::PendingReceiver<network::mojom::URLLoader> loader,
        mojo::PendingRemote<network::mojom::URLLoaderClient> client,
        scoped_refptr<net::HttpResponseHeaders> response_headers) {
    content::CreateFileURLLoader(
        request, std::move(loader), std::move(client),
        nullptr, false, std::move(response_headers));
}


scoped_refptr<net::HttpResponseHeaders> ApplicationUrlLoaderFactory::BuildHttpHeaders() {
    std::string raw_headers;
    raw_headers.append("HTTP/1.1 200 OK");
    raw_headers.append(2, '\0');
    return new net::HttpResponseHeaders(raw_headers);   
}

void ApplicationUrlLoaderFactory::Clone(mojo::PendingReceiver<URLLoaderFactory> factory) {
    receivers_.Add(this, std::move(factory));
}

}   //namespace application

}   //namespace mini_app