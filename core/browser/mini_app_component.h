#ifndef MINI_APP_APP_MINI_APP_COMPONENT_H_
#define MINI_APP_APP_MINI_APP_COMPONENT_H_

namespace content {
class RenderProcessHost;
}

namespace mini_app {

class MiniAppComponent {
public:
    virtual ~MiniAppComponent() {}

protected:
    MiniAppComponent() {}
};

}  // namespace mini_app

#endif  // MINI_APP_APP_MINI_APP_COMPONENT_H_
