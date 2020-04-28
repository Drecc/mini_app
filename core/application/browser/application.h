#ifndef XWALK_APPLICATION_BROWSER_APPLICATION_H_
#define XWALK_APPLICATION_BROWSER_APPLICATION_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list.h"
#include "ui/base/ui_base_types.h"
#include "mini_app/core/application/common/application_data.h"
#include "mini_app/core/browser/app_window/app_window.h"
#include "mini_app/core/browser/app_window/app_window_contents.h"

namespace content {
class RenderProcessHost;
}

namespace mini_app {

class MiniAppBrowserContext;

namespace application {

class Manifest;

class Application : public app_window::AppWindow::Observer {
 public:
  ~Application() override;

  class Observer {
   public:
    virtual void OnApplicationClose(Application* app) {}

   protected:
    virtual ~Observer() {}
  };

  void Terminate();
  bool Init();
  const std::vector<app_window::AppWindow*>& runtimes() const { return windows_; }

  std::string id() const { return data_->ID(); }

  const ApplicationData* data() const { return data_.get(); }
  ApplicationData* data() { return data_.get(); }

  // Tells whether the application use the specified extension.
  bool UseExtension(const std::string& extension_name) const;

  bool RegisterPermissions(const std::string& extension_name,
                           const std::string& perm_table);
  std::string GetRegisteredPermissionName(const std::string& extension_name,
                                          const std::string& api_name) const;

  StoredPermission GetPermission(PermissionType type,
                                 const std::string& permission_name) const;
  bool SetPermission(PermissionType type,
                     const std::string& permission_name,
                     StoredPermission perm);
  bool CanRequestURL(const GURL& url) const;

  void set_observer(Observer* observer) { observer_ = observer; }

  base::WeakPtr<Application> GetWeakPtr() {
    return weak_factory_.GetWeakPtr();
  }

  Application(scoped_refptr<ApplicationData> data,
              MiniAppBrowserContext* context);

  //app_window::AppWindow::Observer
  void OnAppWindowClose(app_window::AppWindow* app_window) override;

 protected:
  virtual bool Launch();
  virtual base::FilePath GetSplashScreenPath();
  virtual GURL GetStartURL(Manifest::Type type) const;

 private:
  // We enforce ApplicationService ownership.
  friend class ApplicationService;
  static std::unique_ptr<Application> Create(scoped_refptr<ApplicationData> data,
      MiniAppBrowserContext* context);
  template <Manifest::Type> GURL GetStartURL() const;
  template <Manifest::Type> void SetWindowShowState(
      app_window::AppWindow::CreateParams* params);
  GURL GetAbsoluteURLFromKey(const std::string& key) const;
  void NotifyTermination();

  app_window::AppWindow::CreateParams window_show_params_;
  MiniAppBrowserContext* browser_context_;
  scoped_refptr<ApplicationData> const data_;
  std::map<std::string, std::string> name_perm_map_;
  std::vector<app_window::AppWindow*> windows_;
  StoredPermissionMap permission_map_;
  Observer* observer_;

  base::WeakPtrFactory<Application> weak_factory_;
  DISALLOW_COPY_AND_ASSIGN(Application);
};

}  // namespace application
}  // namespace mini_app

#endif  // XWALK_APPLICATION_BROWSER_APPLICATION_H_
