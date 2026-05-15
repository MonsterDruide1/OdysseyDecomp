#pragma once

#include <basis/seadTypes.h>

namespace al {
class AnimInfoTable;
struct ActionAnimCtrlInfo;
class InitResourceDataAnim;
class Resource;

class InitResourceDataActionAnim {
public:
    static InitResourceDataActionAnim*
    tryCreate(Resource* resource, const InitResourceDataAnim* dataAnim, const char* suffix);

    InitResourceDataActionAnim(Resource* resource, const InitResourceDataAnim* dataAnim,
                               const char* suffix);
    void sortCtrlInfo();

private:
    s32 mAnimInfoCount = 0;
    ActionAnimCtrlInfo** mAnimInfos = nullptr;
};
}  // namespace al

namespace alAnimFunction {
bool checkPass(f32, f32, f32, bool, f32);
const al::AnimInfoTable* createAnimInfoTableIfNeed(const al::AnimInfoTable* table1,
                                                   const al::AnimInfoTable* table2);
}  // namespace alAnimFunction
