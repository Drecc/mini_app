#ifndef MINI_APP_CORE_APP_WINDOW_COMMON_DRAGGABLE_REGION_H_
#define MINI_APP_CORE_APP_WINDOW_COMMON_DRAGGABLE_REGION_H_

#include "ui/gfx/geometry/rect.h"

namespace mini_app {

namespace app_window {

struct DraggableRegion {
  bool draggable;
  gfx::Rect bounds;

  DraggableRegion();
};

}  // namespace app_window

} // namespace mini_app

#endif  // MINI_APP_CORE_APP_WINDOW_COMMON_DRAGGABLE_REGION_H_
