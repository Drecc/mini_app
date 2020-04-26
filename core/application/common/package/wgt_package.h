// Copyright (c) 2013 Intel Corporation. All rights reserved.
// Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_APPLICATION_COMMON_PACKAGE_WGT_PACKAGE_H_
#define XWALK_APPLICATION_COMMON_PACKAGE_WGT_PACKAGE_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "mini_app/core/application/common/package/package.h"

namespace mini_app {
namespace application {

class WGTPackage : public Package {
 public:
  explicit WGTPackage(const base::FilePath& path);
  ~WGTPackage() override;
  // Returns allowed names of default widget start file.
  static const std::vector<std::string>& GetDefaultWidgetEntryPages();
};

}  // namespace application
}  // namespace mini_app

#endif  // XWALK_APPLICATION_COMMON_PACKAGE_WGT_PACKAGE_H_
