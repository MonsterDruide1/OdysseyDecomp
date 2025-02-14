#include "MapObj/ChurchDoor.h"

#include "Library/Base/StringUtil.h"
#include "Library/Bgm/BgmLineFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "System/GameDataFunction.h"
#include "System/GameDataUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(ChurchDoor, DemoEnterChurch);
NERVE_IMPL(ChurchDoor, CloseWait1);
NERVE_IMPL(ChurchDoor, OpenWait);
NERVE_IMPL(ChurchDoor, Open1);
NERVE_IMPL(ChurchDoor, Open2);
NERVE_IMPL(ChurchDoor, CloseWait2);
NERVE_IMPL(ChurchDoor, CloseWait3);
NERVE_IMPL(ChurchDoor, Open3);

NERVES_MAKE_NOSTRUCT(ChurchDoor, DemoEnterChurch, CloseWait1, OpenWait, Open1, Open2, CloseWait2,
                     CloseWait3, Open3);
}  // namespace

inline bool isCurrentStageMoonWeddingRoom(const al::LiveActor* actor) {
    return al::isEqualString("MoonWorldWeddingRoomStage",
                             GameDataFunction::getCurrentStageName(actor));
}

ChurchDoor::ChurchDoor(const char* name) : al::LiveActor(name) {}

void ChurchDoor::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &CloseWait1, 0);

    if (isCurrentStageMoonWeddingRoom(this)) {
        mSaveObjInfo = rs::createSaveObjInfoWriteSaveData(info);
        if (rs::isOnSaveObjInfo(mSaveObjInfo)) {
            al::invalidateCollisionParts(this);
            al::setNerve(this, &OpenWait);
            makeActorAlive();
            return;
        }
    }

    al::startBgmSituation(this, "CloseChurchDoor", true, true);
    makeActorAlive();
}

bool ChurchDoor::receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) {
    if (rs::isMsgPlayerDisregardTargetMarker(msg))
        return true;

    if (rs::isMsgCapTouchWall(msg)) {
        if ((al::isNerve(this, &Open1) || al::isNerve(this, &Open2)) &&
            al::isLessEqualStep(this, 10))
            return true;

        rs::requestHitReactionToAttacker(msg, other, al::getSensorPos(other));

        if (al::isNerve(this, &CloseWait1)) {
            al::startHitReaction(this, "ヒット1回目");
            al::setNerve(this, &Open1);
        } else if (al::isNerve(this, &CloseWait2) || al::isNerve(this, &Open1)) {
            al::startHitReaction(this, "ヒット2回目");
            al::setNerve(this, &Open2);
        } else if (al::isNerve(this, &CloseWait3) || al::isNerve(this, &Open2)) {
            al::startHitReaction(this, "ヒット3回目");
            al::setNerve(this, &Open3);
        }
        return true;
    }

    return false;
}

bool ChurchDoor::isOpenWait() const {
    return al::isNerve(this, &OpenWait);
}

bool ChurchDoor::isDemoEnterChurch() const {
    return al::isNerve(this, &DemoEnterChurch);
}

void ChurchDoor::startDemoEnterChurch() {
    al::setNerve(this, &DemoEnterChurch);
}

void ChurchDoor::endDemoEnterChurch() {
    al::setNerve(this, &OpenWait);
}

void ChurchDoor::exeCloseWait1() {
    if (al::isFirstStep(this))
        al::startAction(this, "CloseWait1");
}

void ChurchDoor::exeOpen1() {
    if (al::isFirstStep(this))
        al::startAction(this, "Open1");

    al::setNerveAtActionEnd(this, &CloseWait2);
}

void ChurchDoor::exeCloseWait2() {
    if (al::isFirstStep(this))
        al::startAction(this, "CloseWait2");
}

void ChurchDoor::exeOpen2() {
    if (al::isFirstStep(this))
        al::startAction(this, "Open2");

    al::setNerveAtActionEnd(this, &CloseWait3);
}

void ChurchDoor::exeCloseWait3() {
    if (al::isFirstStep(this))
        al::startAction(this, "CloseWait3");
}

void ChurchDoor::exeOpen3() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Open3");
        al::invalidateCollisionParts(this);
    }

    al::setNerveAtActionEnd(this, &OpenWait);
}

void ChurchDoor::exeOpenWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "OpenWait");

        if (isCurrentStageMoonWeddingRoom(this))
            rs::onSaveObjInfo(mSaveObjInfo);
    }
}

void ChurchDoor::exeDemoEnterChurch() {
    if (al::isFirstStep(this)) {
    }
}
