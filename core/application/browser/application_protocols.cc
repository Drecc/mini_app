#include "mini_app/core/application/browser/application_protocols.h"

#include "content/public/browser/web_contents.h"
#include "content/public/browser/browser_context.h"
#include "mini_app/core/application/browser/application_url_loader_factory.h"

namespace mini_app {

namespace application {


std::unique_ptr<network::mojom::URLLoaderFactory>
CreateNonNetworkNavigationURLLoaderFactory(
    content::BrowserContext* browser_context,
    content::WebContents* web_contents) {
    return std::make_unique<ApplicationUrlLoaderFactory>(browser_context, web_contents);
}

}

}