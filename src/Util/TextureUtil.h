#pragma once

namespace al {
class IUseSceneObjHolder;
class LayoutTextureRenderObj;
}  // namespace al

namespace rs {
void registerLayoutTextureRenderObj(al::IUseSceneObjHolder* user,
                                    al::LayoutTextureRenderObj* renderObj);
}
