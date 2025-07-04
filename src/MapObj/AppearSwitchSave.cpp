#include "MapObj/AppearSwitchSave.h"

#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Placement/PlacementFunction.h"

#include "MapObj/RiseMapPartsHolder.h"
#include "System/GameDataUtil.h"
#include "System/SaveObjInfo.h"
#include "Util/SensorMsgFunction.h"

AppearSwitchSave::AppearSwitchSave(al::LiveActor* actor, const al::ActorInitInfo& info)
    : mSwitchActor(actor) {
    mSaveObjInfo = rs::createSaveObjInfoWriteSaveData(info);
    if (rs::isOnSaveObjInfo(mSaveObjInfo))
        mIsOn = true;
    else
        mIsOn = rs::isSequenceTimeBalloonOrRace(actor);

    mAppearTargetNum = al::calcLinkChildNum(info, "AppearTarget");
    mAppearTargetArray = new al::LiveActor*[mAppearTargetNum];
    for (s32 i = 0; i < mAppearTargetNum; i++) {
        mAppearTargetArray[i] = al::createLinksActorFromFactory(info, "AppearTarget", i);
        if (!mAppearTargetArray[i]) {
            mAppearTargetNum = 0;
            break;
        }
        if (mIsOn) {
            if (mAppearTargetArray[i]->getHitSensorKeeper())
                al::sendMsgSwitchOnInit(mAppearTargetArray[i]);
        } else if (mAppearTargetArray[i]->getHitSensorKeeper()) {
            al::sendMsgSwitchOffInit(mAppearTargetArray[i]);
        }
    }

    mKillTargetNum = al::calcLinkChildNum(info, "KillTarget");
    mKillTargetArray = new al::LiveActor*[mKillTargetNum];
    for (s32 i = 0; i < mKillTargetNum; i++) {
        mKillTargetArray[i] = al::createLinksActorFromFactory(info, "KillTarget", i);
        if (mIsOn) {
            if (mKillTargetArray[i]->getHitSensorKeeper())
                al::sendMsgSwitchKillOnInit(mKillTargetArray[i]);
        } else if (mKillTargetArray[i]->getHitSensorKeeper()) {
            al::sendMsgSwitchKillOffInit(mKillTargetArray[i]);
        }
    }

    mRiseMapPartsNum = al::calcLinkChildNum(info, "DemoObjWithRiseParts");
    mRiseMapPartsArray = new RiseMapParts*[mRiseMapPartsNum];
    for (s32 i = 0; i < mRiseMapPartsNum; i++)
        mRiseMapPartsArray[i] =
            (RiseMapParts*)al::createLinksActorFromFactory(info, "DemoObjWithRiseParts", i);

    if (al::isExistLinkChild(info, "RiseParts", 0)) {
        mRiseMapPartsHolder = new RiseMapPartsHolder("隆起パーツ管理者");
        al::initCreateActorWithPlacementInfo(mRiseMapPartsHolder, info);
        if (mIsOn)
            al::sendMsgSwitchOnInit(mRiseMapPartsHolder);
        else
            al::sendMsgSwitchOffInit(mRiseMapPartsHolder);

        mRiseMapPartsHolder->setSwitchActor(mSwitchActor);
        mRiseMapPartsHolder->setRiseMapPartsArray(mRiseMapPartsArray, mRiseMapPartsNum);
    }
}

void AppearSwitchSave::onSwitch() {
    bool isInfoSave = true;
    for (s32 i = 0; i < mAppearTargetNum; i++) {
        if (mAppearTargetArray[i]->getHitSensorKeeper()) {
            if (rs::sendMsgSwitchOnWithSaveRequest(mAppearTargetArray[i], mSaveObjInfo))
                isInfoSave = false;
            else if (mAppearTargetArray[i]->getHitSensorKeeper())
                al::sendMsgSwitchOn(mAppearTargetArray[i]);
        }
    }

    for (s32 i = 0; i < mKillTargetNum; i++)
        if (mKillTargetArray[i]->getHitSensorKeeper())
            al::sendMsgSwitchKillOn(mKillTargetArray[i]);

    if (mRiseMapPartsHolder)
        rs::sendMsgSwitchOnWithSaveRequest(mRiseMapPartsHolder, mSaveObjInfo);
    else if (isInfoSave)
        rs::onSaveObjInfo(mSaveObjInfo);
    mIsOn = true;
}

void AppearSwitchSave::onSwitchDemo() {
    for (s32 i = 0; i < mAppearTargetNum; i++)
        if (mAppearTargetArray[i]->getHitSensorKeeper())
            al::sendMsgSwitchOn(mAppearTargetArray[i]);

    for (s32 i = 0; i < mKillTargetNum; i++)
        if (mKillTargetArray[i]->getHitSensorKeeper())
            al::sendMsgSwitchKillOn(mKillTargetArray[i]);

    if (mRiseMapPartsHolder)
        mRiseMapPartsHolder->startDemo(true);
    rs::onSaveObjInfo(mSaveObjInfo);
    mIsOn = true;
}

bool AppearSwitchSave::isOn() const {
    return mIsOn;
}
