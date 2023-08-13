#pragma once

// TODO: add DrawContext Header to agl
namespace agl {
class DrawContext;
}

namespace al {
struct ExecuteSystemInitInfo {
    agl::DrawContext* mDrawCtx;
};
}  // namespace al
