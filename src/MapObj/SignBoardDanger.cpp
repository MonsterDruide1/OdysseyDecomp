#include "MapObj/SignBoardDanger.h"

#include "Library/Controller/PadRumbleFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "MapObj/CapHanger.h"
#include "MapObj/SignBoardBlow.h"
#include "Util/ItemUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(SignBoardDanger, Wait);
NERVE_IMPL(SignBoardDanger, Reaction);
NERVE_IMPL(SignBoardDanger, Dead);

NERVES_MAKE_STRUCT(SignBoardDanger, Wait, Dead, Reaction);
}  // namespace

SignBoardDanger::SignBoardDanger(const char* name) : al::LiveActor(name) {}

// Mismatch: https://decomp.me/scratch/mqOky
void SignBoardDanger::init(const al::ActorInitInfo& info) {
    al::initMapPartsActor(this, info, nullptr);

    sead::Vector3f upDir;
    al::calcUpDir(&upDir, this);

    sead::Vector3f trans;
    al::getTrans(&trans, info);

    mSignBoardBlow = new SignBoardBlow("壊れモデル", "SignBoardDangerBreak");

    al::initCreateActorNoPlacementInfo(mSignBoardBlow, info);

    mYRotation = al::getRotate(this).y;

    al::initNerve(this, &NrvSignBoardDanger.Wait, 0);

    makeActorAlive();

    // Mismatch starts here
    sead::Vector3f upDir2;
    sead::Vector3f rightDir;
    sead::Vector3f frontDir;
    al::calcUpDir(&upDir2, this);
    al::calcRightDir(&rightDir, this);
    al::calcFrontDir(&frontDir, this);
    al::getTrans(this);

    f32 rotation = mYRotation;
    al::calcUpDir(&upDir2, this);
    al::calcRightDir(&rightDir, this);
    al::calcFrontDir(&frontDir, this);
    sead::Vector3f trans2 = al::getTrans(this);

    sead::Quatf yDegree;
    sead::Quatf zDegree;
    al::makeQuatYDegree(&yDegree, rotation);
    al::makeQuatZDegree(&zDegree, -18.0f);

    sead::Vector3f targetTrans = trans2 + 315.0f * upDir2 + -69.0f * rightDir + 0.0f * frontDir;

    sead::Quatf quat = zDegree * yDegree;
    // Mismatch ends here

    mCapHanger = new CapHanger("CapHanger", false);
    al::initCreateActorNoPlacementInfo(mCapHanger, info);
    al::setTrans(mCapHanger, targetTrans);
    al::setQuat(mCapHanger, quat);
    s32 itemType = rs::getItemType(info);
    if (itemType != -1)
        mCapHanger->initItem(itemType, 1, info);
    mCapHanger->makeActorAlive();
}

bool SignBoardDanger::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                 al::HitSensor* self) {
    if (al::isNerve(this, &NrvSignBoardDanger.Dead)) {
        if (rs::isMsgPlayerDisregardHomingAttack(message) ||
            rs::isMsgPlayerDisregardTargetMarker(message) || al::isMsgPlayerDisregard(message)) {
            return true;
        }

        if (al::isSensorName(self, "Cap") && !al::isSensorCollision(other))
            mRespawnTimer = sead::Mathi::max(mRespawnTimer, 6);

        return false;
    }

    if (rs::isMsgPlayerDisregardHomingAttack(message) || rs::isMsgPlayerDisregardTargetMarker(message))
        return true;

    if (rs::isMsgBreakSignBoard(message)) {
        SignBoardBlow* signBlow = mSignBoardBlow;
        sead::Vector3f upDir = sead::Vector3f::ey;
        sead::Vector3f rightDir;
        sead::Vector3f frontDir;
        al::calcRightDir(&rightDir, this);
        al::calcFrontDir(&frontDir, this);

        sead::Vector3f resetPos =
            al::getTrans(this) + 30.0f * rightDir + 200.0f * upDir + 0.0f * frontDir;
        al::resetPosition(signBlow, resetPos);

        al::setRotateY(signBlow, al::getRotate(this).y);
        alPadRumbleFunction::startPadRumble(this, "破壊汎用（小）", 1000.0f, 3000.0f, -1);
        signBlow->startBlow(al::getActorTrans(other));

        al::LiveActor* subactor = al::getSubActor(this, "残骸モデル");
        subactor->appear();
        al::resetPosition(subactor, al::getTrans(this));

        mCapHanger->kill();
        al::setNerve(this, &NrvSignBoardDanger.Dead);
        return true;
    }

    if (al::isSensorCollision(self) && rs::isMsgCapReflectCollide(message) && isCanStartReaction()) {
        rs::requestHitReactionToAttacker(message, self, other);
        al::setNerve(this, &NrvSignBoardDanger.Reaction);
        return true;
    }
    return false;
}

bool SignBoardDanger::isCanStartReaction() {
    if (al::isNerve(this, &NrvSignBoardDanger.Wait))
        return true;
    return al::isNerve(this, &NrvSignBoardDanger.Reaction) && al::isGreaterEqualStep(this, 30);
}

void SignBoardDanger::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &NrvSignBoardDanger.Dead) && (u32)mRespawnTimer < 5 &&
        al::isSensorName(self, "Cap")) {
        al::sendMsgPush(other, self);
    }
}

void SignBoardDanger::exeWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        al::setRotateY(this, mYRotation);
    }
}

void SignBoardDanger::exeReaction() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Reaction");
        return;
    }

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvSignBoardDanger.Wait);
}

void SignBoardDanger::exeDead() {
    if (al::isFirstStep(this)) {
        al::hideModel(this);
        al::invalidateClipping(this);
        al::invalidateHitSensors(this);
        al::validateHitSensor(this, "Cap");
        al::invalidateCollisionParts(this);
        mRespawnTimer = 180;
    }

    mRespawnTimer--;
    if (mRespawnTimer <= 0) {
        al::setRotateY(this, mYRotation);
        al::showModel(this);
        al::validateClipping(this);
        al::validateHitSensors(this);
        al::validateCollisionParts(this);
        al::setNerve(this, &NrvSignBoardDanger.Wait);
        al::startHitReaction(this, "出現");
        al::getSubActor(this, "残骸モデル")->kill();
        mCapHanger->appear();
        mRespawnTimer = 0;
    }
}
