#ifndef MINI_APP_CORE_COMMON_MINI_APP_CONTENT_CLIENT_H_
#define MINI_APP_CORE_COMMON_MINI_APP_CONTENT_CLIENT_H_

#include <string>
#include "content/public/common/content_client.h"
#include "base/compiler_specific.h"

namespace mini_app {

class MiniAppContentClient : public content::ContentClient {
public:
    MiniAppContentClient();
    ~MiniAppContentClient() override;

    //content::ContentClient
    base::string16 GetLocalizedString(int message_id) override;
    base::StringPiece GetDataResource(
        int resource_id,
        ui::ScaleFactor scale_factor) override;
    base::RefCountedMemory* GetDataResourceBytes(
        int resource_id) override;
    gfx::Image& GetNativeImageNamed(int resource_id) override;
    void AddAdditionalSchemes(Schemes* schemes) override;

};

}   //namespace mini_app

#endif //MINI_APP_CORE_COMMON_MINI_APP_CONTENT_CLIENT_H_
