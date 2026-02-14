#include "MapObj/PoleGrabCeil.h"

#include <basis/seadTypes.h>

#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/CollisionPartsTriangle.h"
#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/KeyMoveFollowTarget.h"
#include "Library/Placement/PlacementFunction.h"

#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(PoleGrabCeil, Wait);
NERVE_IMPL(PoleGrabCeil, Grab);
NERVE_IMPL(PoleGrabCeil, GrabEnd);

NERVES_MAKE_STRUCT(PoleGrabCeil, Wait, Grab, GrabEnd);
}  // namespace

PoleGrabCeil::PoleGrabCeil(const char* name) : al::LiveActor(name) {}

void PoleGrabCeil::init(const al::ActorInitInfo& info) {
    if (al::isObjectName(info, "PoleGrabCeilKeyMoveParts")) {
        al::initActorWithArchiveName(this, info, "PoleGrabCeilKeyMoveParts", "KeyMoveParts");
        mKeyMoveFollowTarget = new al::KeyMoveFollowTarget("ぶらさがり棒[キームーブ移動]本体");
        mKeyMoveFollowTarget->initKeyMoveFollowTarget(info, "PoleGrabCeilKeyMoveParts", nullptr);
        al::initSubActorKeeperNoFile(this, info, 1);
        al::registerSubActor(this, mKeyMoveFollowTarget);
        al::tryGetArg(&mIsCollisionCheck, info, "IsCollisionCheck");
        mSurfaceMatrix.makeIdentity();
        al::setEffectNamedMtxPtr(mKeyMoveFollowTarget, "Surface", &mSurfaceMatrix);
    } else {
        al::initActorWithArchiveName(this, info, "PoleGrabCeil", nullptr);
        mMtxConnector = al::tryCreateMtxConnector(this, info);
        mIsConnectPose = al::tryGetBoolArgOrFalse(info, "IsConnectPose");
    }

    al::initNerve(this, &NrvPoleGrabCeil.Wait, 0);
    al::setHitSensorPosPtr(this, "Snap", &mSnapSensorPos);
    makeActorAlive();
}

void PoleGrabCeil::initAfterPlacement() {
    if (mMtxConnector) {
        sead::Vector3f sideDir = {0.0f, 0.0f, 0.0f};
        al::calcSideDir(&sideDir, this);
        al::attachMtxConnectorToCollision(mMtxConnector, this, al::getTrans(this) + sideDir * 50.0f,
                                          sideDir * -150.0f);
    }
}

void PoleGrabCeil::control() {
    if (mKeyMoveFollowTarget) {
        sead::Vector3f prevPos = al::getTrans(this);
        al::copyPose(this, mKeyMoveFollowTarget);

        if (!mIsCollisionCheck)
            return;

        sead::Vector3f moveVec = al::getTrans(this) - prevPos;
        sead::Vector3f moveDir = {0.0f, 0.0f, 0.0f};

        if (!al::tryNormalizeOrZero(&moveDir, moveVec))
            return;

        const al::ArrowHitInfo* hitInfo = nullptr;

        if (!alCollisionUtil::getFirstPolyOnArrow(this, &hitInfo, prevPos - moveDir,
                                                  moveDir + moveVec, nullptr, nullptr)) {
            if (!alCollisionUtil::getFirstPolyOnArrow(this, &hitInfo, prevPos + moveDir,
                                                      -moveVec - moveDir, nullptr, nullptr)) {
                return;
            }
        }

        const sead::Vector3f& hitPos = alCollisionUtil::getCollisionHitPos(**hitInfo);
        const sead::Vector3f& hitNormal = alCollisionUtil::getCollisionHitPos(**hitInfo);
        al::makeMtxUpNoSupportPos(&mSurfaceMatrix, hitNormal, hitPos);
        al::startHitReaction(mKeyMoveFollowTarget, "コリジョンヒット");
        return;
    }

    if (mMtxConnector) {
        if (mIsConnectPose) {
            al::connectPoseQT(this, mMtxConnector);
            return;
        }
        sead::Vector3f connectTrans = {0.0f, 0.0f, 0.0f};
        al::calcConnectTrans(&connectTrans, mMtxConnector);
        al::updatePoseTrans(this, connectTrans);
    }
}

void followPlayer(sead::Vector3f* pos, al::LiveActor* actor) {
    sead::Vector3f sideDir = {0.0f, 0.0f, 0.0f};
    al::calcSideDir(&sideDir, actor);
    f32 sideOff = sead::Mathf::clamp(sideDir.dot(rs::getPlayerPos(actor) - al::getTrans(actor)),
                                     0.0f, 380.0f);

    pos->set(al::getTrans(actor) + sideOff * sideDir);
}

void PoleGrabCeil::exeWait() {
    followPlayer(&mSnapSensorPos, this);
}

void PoleGrabCeil::exeGrab() {
    followPlayer(&mSnapSensorPos, this);
}

void PoleGrabCeil::exeGrabEnd() {
    followPlayer(&mSnapSensorPos, this);
    if (!al::isLessEqualStep(this, 30))
        al::setNerve(this, &NrvPoleGrabCeil.Wait);
}

void PoleGrabCeil::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (!al::isNerve(this, &NrvPoleGrabCeil.Wait) || rs::isPlayerCollidedGround(this) ||
        al::isInAreaObj(this, "PoleGrabCeilNoSnapArea") || rs::isPlayerCarrySomething(this)) {
        return;
    }

    sead::Vector3f bodyPos = rs::getPlayerBodyPos(this);
    sead::Vector3f sensorPos = al::getSensorPos(self);
    sead::Vector3f sideDir = {0.0f, 0.0f, 0.0f};
    al::calcSideDir(&sideDir, this);
    if (al::isInRange(sideDir.dot(bodyPos - al::getTrans(this)), 0.0f, 380.0f)) {
        f32 scalar = 0.0f;
        sead::Vector3f dir = {0.0f, 0.0f, 0.0f};
        al::separateScalarAndDirection(&scalar, &dir, sensorPos - bodyPos);
        scalar = sead::Mathf::clampMin(scalar * 0.25f, sead::Mathf::clampMax(40.0f, scalar));

        rs::sendMsgObjSnapForce(other, self, dir * scalar);
    }
}

bool PoleGrabCeil::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                              al::HitSensor* self) {
    if (rs::isMsgPlayerDisregardHomingAttack(message))
        return true;

    if (rs::isMsgPlayerStartGrabCeil(message)) {
        if (al::isNerve(this, &NrvPoleGrabCeil.Wait)) {
            al::setNerve(this, &NrvPoleGrabCeil.Grab);
            return true;
        }
        return false;
    }

    if (rs::isMsgPlayerEndGrabCeil(message)) {
        if (al::isNerve(this, &NrvPoleGrabCeil.Grab)) {
            al::setNerve(this, &NrvPoleGrabCeil.GrabEnd);
            return true;
        }
        return false;
    }

    return false;
}
