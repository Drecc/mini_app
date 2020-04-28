#ifndef MINI_APP_CORE_SCOPED_ALLOW_BLOCKING_H_
#define MINI_APP_CORE_SCOPED_ALLOW_BLOCKING_H_

#include "base/threading/thread_restrictions.h"

namespace mini_app {

class MiniAppScopedAllowBlocking {
public:
  MiniAppScopedAllowBlocking() {}

  ~MiniAppScopedAllowBlocking() {}

private:
  base::ScopedAllowBlocking allow_blocking_;
};

}

#endif //  MINI_APP_CORE_SCOPED_ALLOW_BLOCKING_H_