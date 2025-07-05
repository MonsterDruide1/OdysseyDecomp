#pragma once

namespace agl {
class DrawContext;
}

namespace al {

class ExecuteSystemInitInfo {
public:
    ExecuteSystemInitInfo();

private:
    agl::DrawContext* mDrawContext;
};

}  // namespace al
