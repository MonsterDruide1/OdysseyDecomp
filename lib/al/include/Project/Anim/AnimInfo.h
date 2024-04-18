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
public:
    AnimInfoTable(u32);

    AnimResInfo* findAnimInfo(const char* name);
    bool tryFindAnimInfo(const char* name);

    void add(const char* name, void*, f32 frameMax, bool isLoop);
    void sort();

private:
    u32 mInfoCount;
    AnimResInfo* mResInfoStack;
    bool mIsSorted;
};
}  // namespace al
