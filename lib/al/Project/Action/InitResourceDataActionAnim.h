#pragma once

#include <basis/seadTypes.h>

namespace al {
class AnimInfoTable;
struct ActionAnimCtrlInfo;
class InitResourceDataAnim;
class Resource;

class InitResourceDataActionAnim {
public:
    static InitResourceDataActionAnim* tryCreate(Resource*, const InitResourceDataAnim*,
                                                 const char*);

    InitResourceDataActionAnim(Resource*, const InitResourceDataAnim*, const char* resourceYml);
    void sortCtrlInfo();

private:
    s32 mAnimInfoCount = 0;
    ActionAnimCtrlInfo** mAnimInfos = nullptr;
};
}  // namespace al

namespace alAnimFunction {
bool checkPass(f32, f32, f32, bool, f32);
al::AnimInfoTable* createAnimInfoTableIfNeed(const al::AnimInfoTable*, const al::AnimInfoTable*);
}  // namespace alAnimFunction
