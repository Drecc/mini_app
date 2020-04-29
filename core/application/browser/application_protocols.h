#ifndef MINI_APP_CORE_APPLICATION_BROWSER_APPLICATION_PROTOCOLS_H_
#define MINI_APP_CORE_APPLICATION_BROWSER_APPLICATION_PROTOCOLS_H_

#include <memory>
#include <string>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "services/network/public/mojom/url_loader_factory.mojom.h"

namespace content {
class BrowserContext;
class WebContents;
}


namespace mini_app {

namespace application {

std::unique_ptr<network::mojom::URLLoaderFactory>
CreateNonNetworkNavigationURLLoaderFactory(
    content::BrowserContext* browser_context);


}  //namespace application

}   //namespace mini_app

#endif  // MINI_APP_CORE_APPLICATION_BROWSER_APPLICATION_PROTOCOLS_H_