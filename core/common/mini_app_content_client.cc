#include "mini_app/core/common/mini_app_content_client.h"

#include "base/command_line.h"
#include "base/values.h"
#include "mini_app/core/application/common/constants.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/resource/resource_bundle.h"

namespace mini_app {

MiniAppContentClient::MiniAppContentClient() {

}

MiniAppContentClient::~MiniAppContentClient() {

}

base::string16 MiniAppContentClient::GetLocalizedString(int message_id) {
    return l10n_util::GetStringUTF16(message_id);
}

base::StringPiece MiniAppContentClient::GetDataResource(
    int resource_id,
    ui::ScaleFactor scale_factor) {
    return ui::ResourceBundle::GetSharedInstance().GetRawDataResourceForScale(
      resource_id, scale_factor);
}

base::RefCountedMemory* MiniAppContentClient::GetDataResourceBytes(
  int resource_id) {
    return ui::ResourceBundle::GetSharedInstance().LoadDataResourceBytes(
      resource_id);
}

gfx::Image& MiniAppContentClient::GetNativeImageNamed(int resource_id){
    return ui::ResourceBundle::GetSharedInstance().GetNativeImageNamed(
      resource_id);
}

void MiniAppContentClient::AddAdditionalSchemes(Schemes* schemes) {
    schemes->standard_schemes.push_back(application::kApplicationScheme);
}

}
