#include "MapObj/SignBoard.h"

#include <math/seadVector.h>

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/SimpleCircleShadowXZ.h"
#include "Library/Placement/PlacementFunction.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(SignBoard, Wait);
NERVE_IMPL(SignBoard, Reaction);

NERVES_MAKE_NOSTRUCT(SignBoard, Wait, Reaction);
}  // namespace

SignBoard::SignBoard(const char* name) : al::LiveActor(name) {}

void SignBoard::init(const al::ActorInitInfo& info) {
    const char* objectName = nullptr;
    al::getObjectName(&objectName, info);

    bool isWall = al::isEqualString(objectName, "SignBoardNormalWall");
    if (isWall)
        al::initActorWithArchiveName(this, info, "SignBoardNormal", "Wall");
    else
        al::initActor(this, info);

    al::initNerve(this, &Wait, 0);
    SimpleSignBoardFunction::startSignAimVisAnimFromModelName(this, info);

    if (isWall) {
        al::startVisAnimForAction(this, "WallOn");

        sead::Vector3f frontDir;
        al::calcFrontDir(&frontDir, this);

        al::setTrans(this, al::getTrans(this) - frontDir * 16.0f);
    }

    mIsWall = isWall;
    makeActorAlive();
}

void SignBoard::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void SignBoard::exeReaction() {
    if (al::isFirstStep(this))
        al::startAction(this, "Reaction");

    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
}

bool SignBoard::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                           al::HitSensor* self) {
    if (mIsWall ||
        (!rs::isMsgCapReflectCollide(message) && !rs::isMsgPlayerRollingWallHitDown(message))) {
        return false;
    }

    al::setNerve(this, &Reaction);
    rs::requestHitReactionToAttacker(message, self, other);
    return true;
}
