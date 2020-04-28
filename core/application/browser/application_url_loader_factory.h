#ifndef MINI_APP_CORE_APPLICATION_BROWSER_APPLICATION_URL_LOADER_FACTORY_H_
#define MINI_APP_CORE_APPLICATION_BROWSER_APPLICATION_URL_LOADER_FACTORY_H_

#include <memory>
#include <string>

#include "base/callback.h"
#include "base/base_paths.h"
#include "base/memory/ref_counted.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver_set.h"
#include "mojo/public/cpp/bindings/remote.h"
#include "net/http/http_request_headers.h"
#include "net/http/http_response_headers.h"
#include "net/http/http_response_info.h"
#include "services/network/public/mojom/url_loader_factory.mojom.h"
#include "mini_app/core/application/common/application_resource.h"

namespace content {
class BrowserContext;
class WebContents;
}


namespace mini_app {

namespace application {

class ApplicationUrlLoaderFactory : public network::mojom::URLLoaderFactory {
public:
    explicit ApplicationUrlLoaderFactory(
        content::BrowserContext* browser_context,
        content::WebContents* web_contents);
    ~ApplicationUrlLoaderFactory() override;

    void CreateLoaderAndStart(
        mojo::PendingReceiver<::network::mojom::URLLoader> loader, 
        int32_t routing_id, 
        int32_t request_id, 
        uint32_t options, 
        const ::network::ResourceRequest& request, 
        mojo::PendingRemote<::network::mojom::URLLoaderClient> client, 
        const ::net::MutableNetworkTrafficAnnotationTag& traffic_annotation) override;

  
    void Clone(mojo::PendingReceiver<URLLoaderFactory> factory) override;

    static void GetFilePath(const ApplicationResource& resource, base::FilePath* file_path);

    static void OnGetFilePath(
        base::FilePath* file_path, 
        network::ResourceRequest request,
        mojo::PendingReceiver<network::mojom::URLLoader> loader,
        mojo::PendingRemote<network::mojom::URLLoaderClient> client);

    static void StartFileJob(
        network::ResourceRequest request,
        mojo::PendingReceiver<network::mojom::URLLoader> loader,
        mojo::PendingRemote<network::mojom::URLLoaderClient> client,
        scoped_refptr<net::HttpResponseHeaders> response_headers);

    static scoped_refptr<net::HttpResponseHeaders> BuildHttpHeaders();

private:
    // content::BrowserContext* browser_context_;
    // content::WebContents* web_contents_;
    mojo::ReceiverSet<network::mojom::URLLoaderFactory> receivers_;

};


}  //namespace application

}   //namespace mini_app

#endif  // MINI_APP_CORE_APPLICATION_BROWSER_APPLICATION_URL_LOADER_FACTORY_H_