// Copyright (c) 2013 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mini_app/core/application/browser/application.h"

#include <string>

#include "base/files/file_enumerator.h"
#include "base/json/json_reader.h"
#include "base/logging.h"
#include "base/macros.h"
#include "base/memory/ptr_util.h"
#include "base/message_loop/message_loop.h"
#include "base/stl_util.h"
#include "base/strings/string_split.h"
#include "base/threading/thread_restrictions.h"
#include "base/values.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/render_process_host.h"
#include "content/public/browser/site_instance.h"
#include "mini_app/core/application/common/application_manifest_constants.h"
#include "mini_app/core/application/common/constants.h"
#include "mini_app/core/application/common/manifest_handlers/warp_handler.h"
#include "mini_app/core/application/common/package/wgt_package.h"
#include "mini_app/core/browser/mini_app_browser_context.h"
#include "mini_app/core/browser/mini_app_window_client.h"

using content::RenderProcessHost;

namespace mini_app {

namespace keys = application_manifest_keys;
namespace values = application_manifest_values;
namespace widget_keys = application_widget_keys;

namespace {

GURL GetDefaultWidgetEntryPage(
    scoped_refptr<mini_app::application::ApplicationData> data) {
  base::ThreadRestrictions::SetIOAllowed(true);
  base::FileEnumerator iter(
      data->path(), true,
      base::FileEnumerator::FILES,
      FILE_PATH_LITERAL("index.*"));
  const std::vector<std::string>& defaultWidgetEntryPages =
      application::WGTPackage::GetDefaultWidgetEntryPages();
  size_t priority = defaultWidgetEntryPages.size();
  std::string source;

  for (base::FilePath file = iter.Next(); !file.empty(); file = iter.Next()) {
    for (size_t i = 0; i < priority; ++i) {
      if (file.BaseName().MaybeAsASCII() == defaultWidgetEntryPages[i]) {
        source = defaultWidgetEntryPages[i];
        priority = i;
        break;
      }
    }
    if (!priority)
      break;
  }

  return source.empty() ? GURL() : data->GetResourceURL(source);
}

}  // namespace

namespace application {

std::unique_ptr<Application> Application::Create(
    scoped_refptr<ApplicationData> data,
    MiniAppBrowserContext* context) {
  return std::make_unique<Application>(data, context);
}

Application::Application(
    scoped_refptr<ApplicationData> data,
    MiniAppBrowserContext* browser_context)
    : browser_context_(browser_context),
      data_(data),
      observer_(nullptr),
      weak_factory_(this) {
  DCHECK(browser_context_);
  DCHECK(data_.get());
}

Application::~Application() {
  Terminate();
}

template<>
GURL Application::GetStartURL<Manifest::TYPE_WIDGET>() const {
  GURL url = GetAbsoluteURLFromKey(widget_keys::kLaunchLocalPathKey);
  if (url.is_valid())
    return url;

  LOG(WARNING) << "Failed to find start URL from the 'config.xml'"
               << "trying to find default entry page.";
  url = GetDefaultWidgetEntryPage(data_);
  if (url.is_valid())
    return url;

  LOG(WARNING) << "Failed to find a valid start URL in the manifest.";
  return GURL();
}

template<>
GURL Application::GetStartURL<Manifest::TYPE_MANIFEST>() const {
  if (data_->IsHostedApp()) {
    std::string source;
    // Not trying to get a relative path for the "fake" application.
    if (data_->GetManifest()->GetString(keys::kStartURLKey, &source))
      return GURL(source);
    return GURL();
  }

  std::string start_url_source;
  if (data_->GetManifest()->GetString(keys::kStartURLKey, &start_url_source) &&
      !start_url_source.empty()) {
    GURL url(start_url_source);
    if (url.is_valid() && url.SchemeIsHTTPOrHTTPS())
      return url;    
    url = data_->GetResourceURL(start_url_source);   
    if (url.is_valid())
      return url;
  }

  GURL url = GetAbsoluteURLFromKey(keys::kLaunchLocalPathKey);
  if (url.is_valid())
    return url;

  url = GetAbsoluteURLFromKey(keys::kDeprecatedURLKey);
  if (url.is_valid()) {
    LOG(WARNING) << "Deprecated key '" << keys::kDeprecatedURLKey
        << "' found. Please migrate to using '" << keys::kStartURLKey
        << "' instead.";
    return url;
  }

  LOG(WARNING) << "Failed to find a valid start URL in the manifest.";
  return GURL();
}

GURL Application::GetStartURL(Manifest::Type type) const {
  switch (type) {
    case Manifest::Type::TYPE_WIDGET:
      return GetStartURL<Manifest::Type::TYPE_WIDGET>();
    case Manifest::Type::TYPE_MANIFEST:
      return GetStartURL<Manifest::Type::TYPE_MANIFEST>();
    default:
      NOTREACHED() << "Unknown manifest type";
      return GURL();
  }
}

template<>
void Application::SetWindowShowState<Manifest::TYPE_WIDGET>(
    app_window::AppWindow::CreateParams* params) {
  const Manifest* manifest = data_->GetManifest();
  std::string view_modes_string;
  params->state = ui::SHOW_STATE_DEFAULT;
  if (manifest->GetString(widget_keys::kViewModesKey, &view_modes_string)) {
    // FIXME: ATM only 'fullscreen' and 'windowed' values are supported.
    // If the first user prefererence is 'fullscreen', set window show state
    // FULLSCREEN, otherwise set the default window show state.
    std::vector<std::string> modes = base::SplitString(
        view_modes_string, " ", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);
    if (!modes.empty() && modes[0] == "fullscreen")
      params->state = ui::SHOW_STATE_FULLSCREEN;
  }
}

template<>
void Application::SetWindowShowState<Manifest::TYPE_MANIFEST>(
    app_window::AppWindow::CreateParams* params) {
  const Manifest* manifest = data_->GetManifest();
  std::string display_string;

  // FIXME: As we do not support browser mode, the default fallback will be
  // minimal-ui mode.
  params->state = ui::SHOW_STATE_DEFAULT;
  if (!manifest->GetString(keys::kDisplay, &display_string))
    return;

  if (display_string == values::kDisplayModeFullscreen) {
    params->state = ui::SHOW_STATE_FULLSCREEN;
  }
}

bool Application::Launch() {
  if (!windows_.empty()) {
    LOG(ERROR) << "Attempt to launch app with id " << id()
               << ", but it is already running.";
    return false;
  }

  GURL url = GetStartURL(data_->manifest_type());
  if (!url.is_valid())
    return false;

  app_window::AppWindow::CreateParams params;
  data_->manifest_type() == Manifest::TYPE_WIDGET ?
      SetWindowShowState<Manifest::TYPE_WIDGET>(&params) :
      SetWindowShowState<Manifest::TYPE_MANIFEST>(&params);

  auto* window = MiniAppWindowClient::GetInstance()->CreateAppWindow(browser_context_, data_->ID());
  window->AddObserver(this);
  windows_.push_back(window);

  params.window_spec.bounds = data_->window_bounds();
  params.window_spec.maximum_size = data_->window_max_size();
  params.window_spec.minimum_size = data_->window_min_size();
  params.hidden = true;
  params.window_title = data_->Name();

  window_show_params_ = params;
  LOG(INFO) << "load url " << url;
  window->Init(url, new app_window::AppWindowContents(window), params);

  window->Show(app_window::AppWindow::ShowType::SHOW_ACTIVE);

  return true;
}

GURL Application::GetAbsoluteURLFromKey(const std::string& key) const {
  const Manifest* manifest = data_->GetManifest();
  std::string source;

  if (!manifest->GetString(key, &source) || source.empty())
    return GURL();

  return data_->GetResourceURL(source);
}

void Application::Terminate() {
  for (app_window::AppWindow* app : windows_)
    app->Close();
}

void Application::NotifyTermination() {
  if (observer_)
    observer_->OnApplicationClose(this);
}

void Application::OnAppWindowClose(app_window::AppWindow* app_window) {
  NotifyTermination();
}

bool Application::UseExtension(const std::string& extension_name) const {
  // TODO(Bai): Tells whether the application contains the specified extension
  return true;
}

bool Application::RegisterPermissions(const std::string& extension_name,
                                      const std::string& perm_table) {
  // TODO(Bai): Parse the permission table and fill in the name_perm_map_
  // The perm_table format is a simple JSON string, like
  // [{"permission_name":"echo","apis":["add","remove","get"]}]
  std::unique_ptr<base::Value> root = base::JSONReader::ReadDeprecated(perm_table);
  if (root.get() == nullptr || !root->is_list())
    return false;

  base::ListValue* permission_list = static_cast<base::ListValue*>(root.get());
  if (permission_list->GetSize() == 0)
    return false;

  for (base::ListValue::const_iterator iter = permission_list->begin();
      iter != permission_list->end(); ++iter) {
    if (!iter->is_dict())
      return false;

    const base::DictionaryValue* dict_val = nullptr;
    iter->GetAsDictionary(&dict_val);
    std::string permission_name;
    if (!dict_val->GetString("permission_name", &permission_name))
      return false;

    const base::ListValue* api_list = nullptr;
    if (!dict_val->GetList("apis", &api_list))
      return false;

    for (base::ListValue::const_iterator api_iter = api_list->begin();
        api_iter != api_list->end(); ++api_iter) {
      std::string api;
      if (!(api_iter->is_string()
          && api_iter->GetAsString(&api)))
        return false;
      // register the permission and api
      name_perm_map_[api] = permission_name;
      DLOG(INFO) << "Permission Registered [PERM] " << permission_name
                 << " [API] " << api;
    }
  }

  return true;
}

std::string Application::GetRegisteredPermissionName(
    const std::string& extension_name,
    const std::string& api_name) const {
  std::map<std::string, std::string>::const_iterator iter =
      name_perm_map_.find(api_name);
  if (iter == name_perm_map_.end())
    return std::string();
  return iter->second;
}

StoredPermission Application::GetPermission(PermissionType type,
    const std::string& permission_name) const {
  if (type == SESSION_PERMISSION) {
    StoredPermissionMap::const_iterator iter =
        permission_map_.find(permission_name);
    if (iter == permission_map_.end())
      return UNDEFINED_STORED_PERM;
    return iter->second;
  }
  if (type == PERSISTENT_PERMISSION) {
    return data_->GetPermission(permission_name);
  }
  NOTREACHED();
  return UNDEFINED_STORED_PERM;
}

bool Application::SetPermission(PermissionType type,
                                const std::string& permission_name,
                                StoredPermission perm) {
  if (type == SESSION_PERMISSION) {
    permission_map_[permission_name] = perm;
    return true;
  }
  if (type == PERSISTENT_PERMISSION)
    return data_->SetPermission(permission_name, perm);

  NOTREACHED();
  return false;
}

bool Application::CanRequestURL(const GURL& url) const {
  return true;
}

base::FilePath Application::GetSplashScreenPath() {
  return base::FilePath();
}

}  // namespace application
}  // namespace mini_app
