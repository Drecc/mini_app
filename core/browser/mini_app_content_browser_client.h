#ifndef MINI_APP_BROWSER_MINI_APP_CONTENT_BROWSER_CLIENT_H_
#define MINI_APP_BROWSER_MINI_APP_CONTENT_BROWSER_CLIENT_H_

#include "content/public/browser/content_browser_client.h"

namespace content {
class BrowserMainParts;
}

namespace mini_app {

class MiniAppContentBrowserClient : public content::ContentBrowserClient {
public:
    MiniAppContentBrowserClient();
    ~MiniAppContentBrowserClient() override;

    //content::ContentBrowserClient
    std::unique_ptr<content::BrowserMainParts> CreateBrowserMainParts(
      const content::MainFunctionParams& parameters) override;
    void GetAdditionalAllowedSchemesForFileSystem(
      std::vector<std::string>* additional_schemes) override;

private:

};

}  //namespace mini_app


#endif //  MINI_APP_BROWSER_MINI_APP_CONTENT_BROWSER_CLIENT_H_