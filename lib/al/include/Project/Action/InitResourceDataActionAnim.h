#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActionAnimCtrlInfo;
class InitResourceDataAnim;
class Resource;

class InitResourceDataActionAnim {
public:
    static InitResourceDataActionAnim* tryCreate(Resource*, const InitResourceDataAnim*,
                                                 char const*);

    InitResourceDataActionAnim(Resource*, const InitResourceDataAnim*, const char* resourceYml);
    void sortCtrlInfo();

private:
    s32 mInfoCount = 0;
    ActionAnimCtrlInfo** mInfoTable = nullptr;
};
}  // namespace al
