#pragma once

namespace nn::g3d {
class ResMaterialAnim;
}  // namespace nn::g3d

namespace al {
struct AnimResInfo {
    const char* name;
    const nn::g3d::ResMaterialAnim* resMaterialAnim;
    s32 frameMax;
    bool isLoop;
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
    AnimResInfo* mResInfos;
    bool mIsSorted;
};
}  // namespace al
