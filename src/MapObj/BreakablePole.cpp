#include "MapObj/BreakablePole.h"

#include <math/seadVector.h>

#include "Library/Item/ItemUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "System/GameDataUtil.h"
#include "Util/BreakJudgeUtil.h"
#include "Util/ItemUtil.h"

void BreakablePole::init(const al::ActorInitInfo& info) {
    al::BreakMapPartsBase::init(info);

    mShine = rs::tryInitShineByPlacementInfoWithItemMenu(info);
    if (!mShine)
        mHasItem = rs::tryInitItemByPlacementInfo(this, info, false);

    bool isSave = false;
    if (al::tryGetArg(&isSave, info, "IsSave") && isSave) {
        mSaveObjInfo = rs::createSaveObjInfoNoWriteSaveDataInSameWorldResetMiniGame(info);
        mIsSave = true;

        if (mShine || mHasItem)
            makeActorDead();

        if (rs::isOnSaveObjInfo(mSaveObjInfo))
            makeActorDead();
    }
}

void BreakablePole::kill() {
    if (mShine)
        rs::appearPopupShine(mShine, this);
    else if (mHasItem)
        al::appearItem(this);

    al::BreakMapPartsBase::kill();

    if (mIsSave)
        rs::onSaveObjInfo(mSaveObjInfo);
}

bool BreakablePole::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                               al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message))
        return true;

    if (!al::BreakMapPartsBase::receiveMsg(message, other, self))
        return false;

    sead::Vector3f sensorDir = sead::Vector3f::ez;
    al::calcDirBetweenSensorsH(&sensorDir, other, self);

    sead::Vector3f velocityDir = al::getActorVelocity(other);
    velocityDir.y = 0.0f;
    al::tryNormalizeOrZero(&velocityDir);

    sead::Vector3f front = velocityDir + sensorDir;
    al::tryNormalizeOrDirZ(&velocityDir, front);

    front.set(-velocityDir.x, -velocityDir.y, -velocityDir.z);
    al::setFront(this, front);
    al::startHitReaction(this, "破壊");
    return true;
}

al::JudgeFuncPtr BreakablePole::getJudgeFunction(const char* name) const {
    return rs::getBreakJudgeFunction(name);
}
