#include "Project/Action/ActionFlagCtrl.h"

#include "Library/Base/String.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {
ActionFlagCtrl::ActionFlagCtrl(LiveActor* actor, const char* name)
    : mParentActor(actor), mName(createStringIfInStack(name)) {}

ActionFlagCtrl* ActionFlagCtrl::tryCreate(LiveActor* actor, const char* name) {
    if (!isExistModelResource(actor))
        return nullptr;

    sead::FixedSafeString<128> initFileName;
    if (!tryGetActorInitFileName(&initFileName, actor, "ActionFlagCtrl", name))
        createFileNameBySuffix(&initFileName, "ActionFlagCtrl", name);

    if (!isExistModelResourceYaml(actor, initFileName.cstr(), nullptr))
        return nullptr;

    return new ActionFlagCtrl(actor, name);
}
void ActionFlagCtrl::start(const char* name) {
    mLastFlag = findFlagInfo(name);
    mIsBool = false;
    if (!mLastFlag)
        return;

    startCtrlFlag();
    startCtrlSensor();
}
ActionFlagCtrlInfo* ActionFlagCtrl::findFlagInfo(const char* name) const {
    for (s32 i = 0; i < mInfoCount; i++) {
        ActionFlagCtrlInfo* flagInfo = mInfos[i];
        if (isEqualString(flagInfo->mName, name))
            return flagInfo;
    }
    return nullptr;
}
void ActionFlagCtrl::update(f32 frame, f32 frameRate, f32 frameMax, bool isStop) {
    if (!mLastFlag || !mIsBool)
        return;
    if (!mHitSensorKeeper)
        return;

    updateCtrlSensor(frame, frameRate, frameMax, isStop);
}
}  // namespace al
