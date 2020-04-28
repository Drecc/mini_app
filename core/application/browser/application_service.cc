// Copyright (c) 2013 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mini_app/core/application/browser/application_service.h"

#include "base/files/file_util.h"
#include "base/strings/utf_string_conversions.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"
#include "mini_app/core/application/browser/application.h"
#include "mini_app/core/application/common/application_manifest_constants.h"
#include "mini_app/core/application/common/application_file_util.h"
#include "mini_app/core/application/common/id_util.h"

#if defined(OS_WIN)
#include <shobjidl.h>
#endif

namespace mini_app {

namespace application {

ApplicationService* g_application_service = nullptr;

ApplicationService::ApplicationService(MiniAppBrowserContext* browser_context)
  : browser_context_(browser_context) {
  CHECK(!g_application_service);
  g_application_service = this;
}

std::unique_ptr<ApplicationService> ApplicationService::Create(
    MiniAppBrowserContext* browser_context) {
  return std::unique_ptr<ApplicationService>(new ApplicationService(browser_context));
}

ApplicationService* ApplicationService::GetInstance() {
  return g_application_service;
}

ApplicationService::~ApplicationService() {
}

Application* ApplicationService::Launch(
    scoped_refptr<ApplicationData> application_data) {
  if (GetApplicationByID(application_data->ID()) != nullptr) {
    LOG(INFO) << "Application with id: " << application_data->ID()
              << " is already running.";
    // FIXME: we need to notify application that it had been attempted
    // to invoke and let the application to define the further behavior.
    return nullptr;
  }

  Application* application = Application::Create(application_data,
    browser_context_).release();
  
  auto app_iter =
      applications_.insert(applications_.end(), application);

  if (!application->Launch()) {
    applications_.erase(app_iter);
    return nullptr;
  }

  application->set_observer(this);

#if defined (OS_WIN)
  ::SetCurrentProcessExplicitAppUserModelID(
      base::ASCIIToUTF16(application->data()->Name()).c_str());
#endif

  for(Observer& ob : observers_) {
    ob.DidLaunchApplication(application);
  }

  application->Init();

  return application;
}

Application* ApplicationService::LaunchFromManifestPath(
    const base::FilePath& path, Manifest::Type manifest_type) {
  std::string error;
  std::unique_ptr<Manifest> manifest = LoadManifest(path, manifest_type, &error);
  if (!manifest) {
    LOG(ERROR) << "Failed to load manifest.";
    return nullptr;
  }

  base::FilePath app_path = path.DirName();
  LOG(INFO) << "Loading app from " << app_path.MaybeAsASCII();
  std::string app_id = GenerateIdForPath(app_path);
#if defined (OS_WIN)
  std::string update_id;
  if (manifest->GetString(application_manifest_keys::kXWalkWindowsUpdateID,
                          &update_id)) {
    app_id = GenerateId(update_id);
  }
#endif
  scoped_refptr<ApplicationData> application_data = ApplicationData::Create(
      app_path, app_id, ApplicationData::LOCAL_DIRECTORY,
      std::move(manifest), &error);
  if (!application_data.get()) {
    LOG(ERROR) << "Error occurred while trying to load application: "
               << error;
    return nullptr;
  }

  return Launch(application_data);
}

Application* ApplicationService::LaunchFromPackagePath(
    const base::FilePath& path) {
  std::unique_ptr<Package> package = Package::Create(path);
  if (!package || !package->IsValid()) {
    LOG(ERROR) << "Failed to obtain valid package from "
               << path.AsUTF8Unsafe();
    return nullptr;
  }

  base::FilePath tmp_dir, target_dir;
  if (!GetTempDir(&tmp_dir)) {
    LOG(ERROR) << "Failed to obtain system temp directory.";
    return nullptr;
  }

#if defined (OS_WIN)
  base::CreateTemporaryDirInDir(tmp_dir,
      base::UTF8ToWide(package->name()), &target_dir);
#else
  base::CreateTemporaryDirInDir(tmp_dir, package->name(), &target_dir);
#endif
  if (!package->ExtractTo(target_dir)) {
    LOG(ERROR) << "Failed to unpack to a temporary directory: "
               << target_dir.MaybeAsASCII();
    return nullptr;
  }

  std::string app_id;
  if (package->manifest_type() == Manifest::TYPE_MANIFEST)
    app_id = package->Id();
  std::string error;
  scoped_refptr<ApplicationData> application_data = LoadApplication(
      target_dir, app_id, ApplicationData::TEMP_DIRECTORY,
      package->manifest_type(), &error);
  if (!application_data.get()) {
    LOG(ERROR) << "Error occurred while trying to load application: "
               << error;
    return nullptr;
  }

  return Launch(application_data);
}

// Launch an application created from arbitrary url.
// FIXME: This application should have the same strict permissions
// as common browser apps.
Application* ApplicationService::LaunchHostedURL(const GURL& url) {
  const std::string& url_spec = url.spec();
  if (url_spec.empty()) {
      LOG(ERROR) << "Failed to launch application from the URL: " << url;
      return nullptr;
  }

  const std::string& app_id = GenerateId(url_spec);

  std::unique_ptr<base::DictionaryValue> settings(new base::DictionaryValue());
  // FIXME: define permissions!
  settings->SetString(application_manifest_keys::kStartURLKey, url_spec);
  // FIXME: Why use URL as name?
  settings->SetString(application_manifest_keys::kNameKey, url_spec);
  settings->SetString(application_manifest_keys::kXWalkVersionKey, "0");

  std::unique_ptr<Manifest> manifest(
      new Manifest(std::move(settings), Manifest::TYPE_MANIFEST));

  std::string error;
  scoped_refptr<ApplicationData> app_data =
        ApplicationData::Create(base::FilePath(), app_id,
        ApplicationData::EXTERNAL_URL, std::move(manifest), &error);
  DCHECK(app_data.get());

  return Launch(app_data);
}

namespace {

struct ApplicationRenderHostIDComparator {
    explicit ApplicationRenderHostIDComparator(int id) : id(id) { }
    bool operator() (Application* application) {
      // return id == application->GetRenderProcessHostID();
      return false;
    }
    int id;
};

struct ApplicationIDComparator {
    explicit ApplicationIDComparator(const std::string& app_id)
      : app_id(app_id) { }
    bool operator() (Application* application) {
      return app_id == application->id();
    }
    std::string app_id;
};

}  // namespace

Application* ApplicationService::GetApplicationByRenderHostID(int id) const {
  ApplicationRenderHostIDComparator comparator(id);
  auto found = std::find_if(
      applications_.begin(), applications_.end(), comparator);
  if (found != applications_.end())
    return *found;
  return nullptr;
}

Application* ApplicationService::GetApplicationByID(
    const std::string& app_id) const {
  ApplicationIDComparator comparator(app_id);
  auto found = std::find_if(
      applications_.begin(), applications_.end(), comparator);
  if (found != applications_.end())
    return *found;
  return nullptr;
}

void ApplicationService::AddObserver(Observer* observer) {
  observers_.AddObserver(observer);
}

void ApplicationService::RemoveObserver(Observer* observer) {
  observers_.RemoveObserver(observer);
}

void ApplicationService::OnApplicationClose(
                                      Application* application) {
  auto found = std::find(
      applications_.begin(), applications_.end(), application);
  CHECK(found != applications_.end());
  for(Observer& ob : observers_) {
    ob.WillDestroyApplication(application);
  }
  scoped_refptr<ApplicationData> app_data = application->data();
  applications_.erase(found);
  if(applications_.size() <= 0) {
    std::move(quit_main_message_loop_).Run();
  }
  // if (app_data->source_type() == ApplicationData::TEMP_DIRECTORY) {
  //     LOG(INFO) << "Deleting the app temporary directory "
  //               << app_data->path().AsUTF8Unsafe();
  //     base::PostTask(FROM_HERE, content::BrowserThread::IO,
  //          base::Bind(base::IgnoreResult(&base::DeleteFile),
  //                               app_data->path(), true /*recursive*/));
  // }
}

void ApplicationService::SetMainMessageLoopQuitClosure(base::OnceClosure quit_closure) {
  quit_main_message_loop_ = std::move(quit_closure);
}

void ApplicationService::CheckAPIAccessControl(const std::string& app_id,
    const std::string& extension_name,
    const std::string& api_name, const PermissionCallback& callback) {
  Application* app = GetApplicationByID(app_id);
  if (!app) {
    LOG(ERROR) << "No running application found with ID: "
      << app_id;
    callback.Run(UNDEFINED_RUNTIME_PERM);
    return;
  }
  if (!app->UseExtension(extension_name)) {
    LOG(ERROR) << "Can not find extension: "
      << extension_name << " of Application with ID: "
      << app_id;
    callback.Run(UNDEFINED_RUNTIME_PERM);
    return;
  }
  // Permission name should have been registered at extension initialization.
  std::string permission_name =
      app->GetRegisteredPermissionName(extension_name, api_name);
  if (permission_name.empty()) {
    LOG(ERROR) << "API: " << api_name << " of extension: "
      << extension_name << " not registered!";
    callback.Run(UNDEFINED_RUNTIME_PERM);
    return;
  }
  // Okay, since we have the permission name, let's get down to the policies.
  // First, find out whether the permission is stored for the current session.
  StoredPermission perm = app->GetPermission(
      SESSION_PERMISSION, permission_name);
  if (perm != UNDEFINED_STORED_PERM) {
    // "PROMPT" should not be in the session storage.
    DCHECK(perm != PROMPT);
    if (perm == ALLOW) {
      callback.Run(ALLOW_SESSION);
      return;
    }
    if (perm == DENY) {
      callback.Run(DENY_SESSION);
      return;
    }
    NOTREACHED();
  }
  // Then, query the persistent policy storage.
  perm = app->GetPermission(PERSISTENT_PERMISSION, permission_name);
  // Permission not found in persistent permission table, normally this should
  // not happen because all the permission needed by the application should be
  // contained in its manifest, so it also means that the application is asking
  // for something wasn't allowed.
  if (perm == UNDEFINED_STORED_PERM) {
    callback.Run(UNDEFINED_RUNTIME_PERM);
    return;
  }
  if (perm == PROMPT) {
    // TODO(Bai): We needed to pop-up a dialog asking user to chose one from
    // either allow/deny for session/one shot/forever. Then, we need to update
    // the session and persistent policy accordingly.
    callback.Run(UNDEFINED_RUNTIME_PERM);
    return;
  }
  if (perm == ALLOW) {
    callback.Run(ALLOW_ALWAYS);
    return;
  }
  if (perm == DENY) {
    callback.Run(DENY_ALWAYS);
    return;
  }
  NOTREACHED();
}

bool ApplicationService::RegisterPermissions(const std::string& app_id,
    const std::string& extension_name,
    const std::string& perm_table) {
  Application* app = GetApplicationByID(app_id);
  if (!app) {
    LOG(ERROR) << "No running application found with ID: " << app_id;
    return false;
  }
  if (!app->UseExtension(extension_name)) {
    LOG(ERROR) << "Can not find extension: "
               << extension_name << " of Application with ID: "
               << app_id;
    return false;
  }
  return app->RegisterPermissions(extension_name, perm_table);
}

}  // namespace application
}  // namespace mini_app
