#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
}  // namespace al

class CapMessageAreaChecker;
class CapMessageEnableChecker;
class CapMessageGameDataChecker;
class CapMessageShowInfo;
struct SaveObjInfo;

class CapMessagePlacement : public al::LiveActor {
public:
    CapMessagePlacement(const char* name);

    void init(const al::ActorInitInfo& info);
    void onKill();
    void onAppear();
    void initAfterPlacement();
    void showStart();
    void trySave();
    void showEnd();
    bool isActive() const;
    bool checkEnbale() const;

    const CapMessageShowInfo* getShowInfo() const { return mShowInfo; }

    s32 getAppearPriority() const { return mAppearPriority; }

    s32 getDelayTime() const { return mDelayTime; }

    bool isDelayCancel() const { return mIsDelayCancel; }

    bool isKeepDisp() const { return mIsKeepDisp; }

private:
    CapMessageGameDataChecker* mGameDataChecker = nullptr;
    CapMessageAreaChecker* mAreaChecker = nullptr;
    CapMessageShowInfo* mShowInfo = nullptr;
    sead::FixedSafeString<0x100> mMessageName;
    sead::FixedSafeString<0x100> mLabelName;
    bool mIsStageMessage = false;
    u8 _351[7] = {};
    SaveObjInfo* mSaveObjInfo = nullptr;
    s32 mAppearPriority = 0;
    s32 mStartNum = 0;
    s32 mDelayTime = 0;
    bool mIsDelayCancel = false;
    bool mIsSaveObjInfoOn = false;
    bool mIsAppearSwitchOff = false;
    bool mIsKilledBySwitch = false;
    bool mIsKeepDisp = false;
};
