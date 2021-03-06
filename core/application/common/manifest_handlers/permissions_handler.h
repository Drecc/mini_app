// Copyright (c) 2013 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_APPLICATION_COMMON_MANIFEST_HANDLERS_PERMISSIONS_HANDLER_H_
#define XWALK_APPLICATION_COMMON_MANIFEST_HANDLERS_PERMISSIONS_HANDLER_H_

#include <string>
#include <vector>

#include "mini_app/core/application/common/manifest_handler.h"
#include "mini_app/core/application/common/permission_types.h"

namespace mini_app {
namespace application {

class PermissionsInfo: public ApplicationData::ManifestData {
 public:
  PermissionsInfo();
  ~PermissionsInfo() override;

  const PermissionSet& GetAPIPermissions() const {
    return api_permissions_;}
  void SetAPIPermissions(const PermissionSet& api_permissions) {
    api_permissions_ = api_permissions;
  }

 private:
  PermissionSet api_permissions_;
  DISALLOW_COPY_AND_ASSIGN(PermissionsInfo);
};

class PermissionsHandler: public ManifestHandler {
 public:
  PermissionsHandler();
  ~PermissionsHandler() override;

  bool Parse(scoped_refptr<ApplicationData> application,
             base::string16* error) override;
  bool AlwaysParseForType(Manifest::Type type) const override;
  std::vector<std::string> Keys() const override;

 private:
  DISALLOW_COPY_AND_ASSIGN(PermissionsHandler);
};

}  // namespace application
}  // namespace mini_app

#endif  // XWALK_APPLICATION_COMMON_MANIFEST_HANDLERS_PERMISSIONS_HANDLER_H_
