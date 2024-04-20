#pragma once

#include <basis/seadTypes.h>

namespace al {
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
