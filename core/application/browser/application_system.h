#ifndef MINI_APP_APP_APPLICATION_BROWSER_APPLICATION_SYSTEM_H_
#define MINI_APP_APP_APPLICATION_BROWSER_APPLICATION_SYSTEM_H_

#include <map>
#include <memory>

#include "base/memory/ref_counted.h"

class GURL;

namespace base {
class CommandLine;
}

namespace content {
class RenderProcessHost;
}

namespace mini_app {

class MiniAppBrowserContext;

namespace application {

class ApplicationService;

class ApplicationSystem {
 public:
  virtual ~ApplicationSystem();

  static std::unique_ptr<ApplicationSystem> Create(
      MiniAppBrowserContext* browser_context);

  // The ApplicationService is created at startup.
  ApplicationService* application_service() {
    return application_service_.get();
  }

  // Launches an application based on the given command line, there are
  // different ways to inform which application should be launched
  //
  // (1) app_id from the binary name;
  // (2) launching a directory that contains an extracted package.
  // (3) launching from the path to the packaged application file.
  //
  // The parameter `url` contains the current URL Crosswalk is considering to
  // load, and the output parameter `run_default_message_loop` controls whether
  // Crosswalk should run the mainloop or not.
  //
  // A return value of true indicates that ApplicationSystem handled the command
  // line, so the caller shouldn't try to load the url by itself.
  virtual bool LaunchFromCommandLine(const GURL& url);
  void SetMainMessageLoopQuitClosure(base::OnceClosure quit_closure);
 protected:
  explicit ApplicationSystem(MiniAppBrowserContext* browser_context);

  // Note: initialization order matters.
  MiniAppBrowserContext* browser_context_;
  std::unique_ptr<ApplicationService> application_service_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ApplicationSystem);
};

}  // namespace application
}  // namespace mini_app

#endif  // MINI_APP_APP_APPLICATION_BROWSER_APPLICATION_SYSTEM_H_
