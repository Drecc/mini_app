#include "mini_app/core/browser/mini_app_content_browser_client.h"

#include "base/logging.h"
#include "content/public/browser/render_process_host.h"
#include "content/public/browser/web_contents.h"
#include "mini_app/core/application/browser/application_protocols.h"
#include "mini_app/core/application/common/constants.h"
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

void MiniAppContentBrowserClient::RegisterNonNetworkNavigationURLLoaderFactories(
      int frame_tree_node_id,
      content::ContentBrowserClient::NonNetworkURLLoaderFactoryMap* factories) {
  content::WebContents* web_contents =
      content::WebContents::FromFrameTreeNodeId(frame_tree_node_id);
  factories->emplace(
      application::kApplicationScheme,
      application::CreateNonNetworkNavigationURLLoaderFactory(
          web_contents->GetBrowserContext()));
}

void MiniAppContentBrowserClient::RegisterNonNetworkSubresourceURLLoaderFactories(
    int render_process_id,
    int render_frame_id,
    content::ContentBrowserClient::NonNetworkURLLoaderFactoryMap* factories) {
    content::RenderProcessHost* process_host =
        content::RenderProcessHost::FromID(render_process_id);
    content::BrowserContext* browser_context = process_host->GetBrowserContext();
    factories->emplace(
        application::kApplicationScheme,
        application::CreateNonNetworkNavigationURLLoaderFactory(
            browser_context));            
}

}   //namespace mini_app