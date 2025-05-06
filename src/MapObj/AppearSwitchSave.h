#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActorInitInfo;
class LiveActor;
}  // namespace al
class RiseMapParts;
class RiseMapPartsHolder;
class SaveObjInfo;

class AppearSwitchSave {
public:
    AppearSwitchSave(al::LiveActor* actor, const al::ActorInitInfo& info);
    void onSwitch();
    void onSwitchDemo();
    bool isOn() const;

private:
    al::LiveActor* mSwitchActor = nullptr;
    RiseMapPartsHolder* mRiseMapPartsHolder = nullptr;
    al::LiveActor** mAppearTargetArray = nullptr;
    s32 mAppearTargetNum = 0;
    RiseMapParts** mRiseMapPartsArray = nullptr;
    s32 mRiseMapPartsNum = 0;
    al::LiveActor** mKillTargetArray = nullptr;
    s32 mKillTargetNum = 0;
    SaveObjInfo* mSaveObjInfo = nullptr;
    bool mIsOn = false;
};

static_assert(sizeof(AppearSwitchSave) == 0x50);
