#pragma once

namespace al {
class AnimInfoTable;
class Resource;

class InitResourceDataAnim {
public:
    static InitResourceDataAnim* tryCreate(Resource*, Resource*);

    InitResourceDataAnim(Resource*, AnimInfoTable*, AnimInfoTable*, AnimInfoTable*, AnimInfoTable*,
                         AnimInfoTable*);

private:
    AnimInfoTable* mInfoTable;
    AnimInfoTable* mFclAnim;
    AnimInfoTable* mFtsAnim;
    AnimInfoTable* mFtpAnim;
    AnimInfoTable* mInfoTable2;
};
}  // namespace al
