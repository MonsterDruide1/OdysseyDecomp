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
    if (0 < mInfoCount) {
        for (int i = 0; i < mInfoCount; i++) {
            ActionFlagCtrlInfo* flagInfo = mInfoTable[i];

            if (isEqualStringCase(flagInfo->mName, name)) {
                mLastFlag = flagInfo;
                mBool = false;
                if (!(flagInfo))
                    return;

                startCtrlFlag();
                startCtrlSensor();
                return;
            }
        }
    }
    mLastFlag = nullptr;
    mBool = false;
}
ActionFlagCtrlInfo* ActionFlagCtrl::findFlagInfo(const char* name) const {
    if (0 > mInfoCount)
        return nullptr;

    for (int i = 0; i < mInfoCount; i++) {
        ActionFlagCtrlInfo* flagInfo = mInfoTable[i];
        if (isEqualString(flagInfo->mName, name))
            return flagInfo;
    }
    return nullptr;
}
void ActionFlagCtrl::update(float frame, float frameRate, float frameMax, bool isStop) {
    if (mLastFlag && mBool)
        if (mHitSensorKeeper)
            updateCtrlSensor(frame, frameRate, frameMax, isStop);
}
}  // namespace al
