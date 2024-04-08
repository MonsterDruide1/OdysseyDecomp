#pragma once

#include <basis/seadTypes.h>

namespace nn::g3d {
class ResMaterialAnim;
}  // namespace nn::g3d

namespace al {
struct AnimResInfo {
    const char* mName;
    const nn::g3d::ResMaterialAnim* mResMaterialAnim;
    s32 mFrameMax;
    bool mIsLoop;
};

class AnimInfoTable {
    u32 mInfoCount;
    AnimResInfo* mResInfoStack;
    bool _10;
};
}  // namespace al
