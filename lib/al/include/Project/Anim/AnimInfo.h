#pragma once

namespace nn {
namespace g3d {
class ResMaterialAnim;
}
}  // namespace nn

namespace al {
struct AnimResInfo {
    void* _0;
    const nn::g3d::ResMaterialAnim* mResMaterialAnim;
    int mFrameMax;
    bool mIsLoop;
};

class AnimInfoTable {
    int mInfoCount;
    AnimResInfo* mResInfoStack;
    bool mIsBool;
};
}  // namespace al