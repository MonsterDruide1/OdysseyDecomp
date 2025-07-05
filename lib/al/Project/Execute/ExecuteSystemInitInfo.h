#pragma once

namespace agl {
class DrawContext;
}

namespace al {
struct ExecuteSystemInitInfo {
    ExecuteSystemInitInfo();

    agl::DrawContext* drawCtx;
};

static_assert(sizeof(ExecuteSystemInitInfo) == 0x8);

}  // namespace al
