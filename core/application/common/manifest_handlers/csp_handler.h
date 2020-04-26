#ifndef XWALK_APPLICATION_COMMON_MANIFEST_HANDLERS_CSP_HANDLER_H_
#define XWALK_APPLICATION_COMMON_MANIFEST_HANDLERS_CSP_HANDLER_H_

#include <map>
#include <string>
#include <vector>

#include "mini_app/core/application/common/application_data.h"
#include "mini_app/core/application/common/manifest_handler.h"

namespace mini_app {
namespace application {

class CSPInfo : public ApplicationData::ManifestData {
 public:
  CSPInfo();
  ~CSPInfo() override;

  void SetDirective(const std::string& directive_name,
                    const std::vector<std::string>& directive_value) {
    policies_[directive_name] = directive_value;
  }
  const std::map<std::string, std::vector<std::string> >&
      GetDirectives() const { return policies_; }

 private:
  std::map<std::string, std::vector<std::string> > policies_;
};

class CSPHandler : public ManifestHandler {
 public:
  explicit CSPHandler(Manifest::Type type);
  ~CSPHandler() override;

  bool Parse(scoped_refptr<ApplicationData> application,
             base::string16* error) override;
  bool AlwaysParseForType(Manifest::Type type) const override;
  std::vector<std::string> Keys() const override;

 private:
  Manifest::Type type_;

  DISALLOW_COPY_AND_ASSIGN(CSPHandler);
};

}  // namespace application
}  // namespace mini_app

#endif  // XWALK_APPLICATION_COMMON_MANIFEST_HANDLERS_CSP_HANDLER_H_
