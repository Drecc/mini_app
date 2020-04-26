#include "mini_app/core/browser/mini_app_content_browser_client.h"

#include "base/logging.h"
#include "mini_app/core/application/common/constants.h"
#include "mini_app/core/browser/mini_app_browser_main_parts.h"

namespace mini_app {

MiniAppContentBrowserClient::MiniAppContentBrowserClient() {
    LOG(INFO) << "MiniAppContentBrowserClient::MiniAppContentBrowserClient";
}

MiniAppContentBrowserClient::~MiniAppContentBrowserClient() {
    LOG(INFO) << "MiniAppContentBrowserClient::~MiniAppContentBrowserClient";
}

std::unique_ptr<content::BrowserMainParts> MiniAppContentBrowserClient::CreateBrowserMainParts(
      const content::MainFunctionParams& parameters) {
    LOG(INFO) << "MiniAppContentBrowserClient::CreateBrowserMainParts";
    auto parts = std::make_unique<MiniAppBrowserMainParts>(parameters);
    return parts;
}

void MiniAppContentBrowserClient::GetAdditionalAllowedSchemesForFileSystem(
    std::vector<std::string>* additional_schemes) {
    additional_schemes->push_back(application::kApplicationScheme);
}

}   //namespace mini_app