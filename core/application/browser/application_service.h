// Copyright (c) 2013 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_APPLICATION_BROWSER_APPLICATION_SERVICE_H_
#define XWALK_APPLICATION_BROWSER_APPLICATION_SERVICE_H_

#include <memory>
#include <string>

#include "base/files/file_path.h"
#include "base/observer_list.h"
#include "mini_app/core/application/browser/application.h"
#include "mini_app/core/application/common/permission_policy_manager.h"
#include "mini_app/core/application/common/application_data.h"

namespace mini_app {

class MiniAppBrowserContext;

namespace application {

// The application service manages launch and termination of the applications.
class ApplicationService : public Application::Observer {
 public:
  // Client code may use this class (and register with AddObserver below) to
  // keep track of applications life cycle.
  class Observer : public base::CheckedObserver {
   public:
    virtual void DidLaunchApplication(Application* app) {}
    virtual void WillDestroyApplication(Application* app) {}
   protected:
    ~Observer() override {}
  };

  ~ApplicationService() override;

  static std::unique_ptr<ApplicationService> Create(
    MiniAppBrowserContext* browser_context);

  // Launch an unpacked application using path to the manifest file
  // of an unpacked application.
  Application* LaunchFromManifestPath(const base::FilePath& path,
                                      Manifest::Type manifest_type);

  // Launch an application using path to its package file.
  // Note: the given package is unpacked to a temporary folder,
  // which is deleted after the application terminates.
  Application* LaunchFromPackagePath(const base::FilePath& path);

  // Launch an application from an arbitrary URL.
  // Creates a "dummy" application.
  Application* LaunchHostedURL(const GURL& url);

  Application* GetApplicationByRenderHostID(int id) const;
  Application* GetApplicationByID(const std::string& app_id) const;

  const std::vector<Application*>& active_applications() const {
      return applications_; }

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);
  void SetMainMessageLoopQuitClosure(base::OnceClosure quit_closure);

  // Check whether application has permission to access API of extension.
  void CheckAPIAccessControl(const std::string& app_id,
      const std::string& extension_name,
      const std::string& api_name, const PermissionCallback& callback);
  // Register APIs implemented by extension. This method will be called
  // when application register extensions.
  // Parameter perm_table is a string which is a map between extension
  // and it includes APIs. For example perm_table is like '{"bluetooth":
  // ["read", "write", "management"]}'.
  bool RegisterPermissions(const std::string& app_id,
      const std::string& extension_name,
      const std::string& perm_table);

 protected:
  explicit ApplicationService(MiniAppBrowserContext* browser_context);

  Application* Launch(scoped_refptr<ApplicationData> application_data);

 private:
  // Implementation of Application::Observer.
  void OnApplicationClose(Application* app) override;

  base::OnceClosure quit_main_message_loop_;
  MiniAppBrowserContext* browser_context_;
  std::vector<Application*> applications_;
  base::ObserverList<Observer> observers_;

  DISALLOW_COPY_AND_ASSIGN(ApplicationService);
};

}  // namespace application
}  // namespace mini_app

#endif  // XWALK_APPLICATION_BROWSER_APPLICATION_SERVICE_H_
