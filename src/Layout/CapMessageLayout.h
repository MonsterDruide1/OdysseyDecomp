#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}  // namespace al

class CapMessageShowInfo;

class CapMessageLayout : public al::LayoutActor {
public:
    CapMessageLayout(const char* name, const char* layoutName, const al::LayoutInitInfo& initInfo,
                     const char* archiveName, bool isLocalized);

    void appear() override;
    void kill() override;

    void end();
    void start(const CapMessageShowInfo* showInfo);
    void startCore(const CapMessageShowInfo* showInfo, bool isContinue);
    void startContinue(const CapMessageShowInfo* showInfo);
    void setContinueShow(bool isContinueShow);
    void tryEnd();
    bool tryEndExistContinueDemo();
    void exeDelay();
    void exeAppear();
    void exeFollowText();
    void exeWait();
    void exeEnd();
    bool isDelay(const char* labelName) const;
    bool isShow(const char* labelName) const;

private:
    sead::FixedSafeString<0x100> mMessageName;
    sead::FixedSafeString<0x100> mLabelName;
    char _360[0x230];
    sead::FixedSafeString<0x40> mReplaceTagName;
    s32 mReplaceNumber = 0;
    bool mIsContinueShow = false;
    u8 _5ed[3] = {};
};

static_assert(sizeof(CapMessageLayout) == 0x5F0, "CapMessageLayout");
