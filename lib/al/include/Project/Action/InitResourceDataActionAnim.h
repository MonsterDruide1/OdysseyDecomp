#pragma once

#include <basis/seadTypes.h>

namespace al {
class ActionAnimCtrlInfo;
class InitResourceDataAnim;
class Resource;

class InitResourceDataActionAnim {
public:
    static InitResourceDataActionAnim* tryCreate(Resource*, InitResourceDataAnim const*,
                                                 char const*);

    InitResourceDataActionAnim(Resource*, InitResourceDataAnim const*, char const* resourceYml);
    void sortCtrlInfo(void);

private:
    s32 mLength = 0;
    ActionAnimCtrlInfo** mInfoStack;  // ActionAnimCtrlInfo*[mLength];
};
}  // namespace al
