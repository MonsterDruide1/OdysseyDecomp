#pragma once

namespace al {
class InitResourceDataAnim;
class Resource;
struct ActionAnimCtrlInfo;

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
