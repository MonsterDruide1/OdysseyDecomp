#include "Enemy/Utsubo.h"

#include <math/seadMathCalcCommon.h>

#include "Library/Area/AreaObjUtil.h"
#include "Library/Base/StringUtil.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/MatrixUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Shadow/ActorShadowUtil.h"

#include "Enemy/UtsuboFunction.h"
#include "Item/Shine.h"
#include "Item/ShineChip.h"
#include "Util/ItemUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {

const sead::Vector3f cLinkedShineLocalOffset(0.0f, -570.0f, -420.0f);
const UtsuboFunction::UtsuboAttackParam cAttackParam = {120, 200.0f, 800.0f, 2000.0f, 100.0f};

NERVE_IMPL(Utsubo, WaitForWatcher)
NERVE_IMPL(Utsubo, Wait)
NERVE_IMPL(Utsubo, Rise)
NERVE_IMPL(Utsubo, AttackSign)
NERVE_IMPL(Utsubo, Attack)
NERVE_IMPL(Utsubo, Sink)
NERVE_IMPL(Utsubo, RiseSign)
NERVE_IMPL(Utsubo, Move)
NERVE_IMPL(Utsubo, Follow)
NERVE_IMPL(Utsubo, WaitForce)
NERVES_MAKE_STRUCT(Utsubo, WaitForWatcher, Wait, Rise, AttackSign, Attack, Sink, RiseSign, Move,
                   Follow, WaitForce)

static inline bool isUpright(const al::LiveActor* actor) {
    sead::Vector3f upDir;
    al::calcUpDir(&upDir, actor);
    return al::isNear(upDir, sead::Vector3f::ey, 0.001f);
}

static void calcBodySensorPos(sead::Vector3f* out, const al::LiveActor* actor,
                              const sead::Vector3f& target, f32 riseDistance) {
    sead::Vector3f reverseUpDir;
    al::calcUpDir(&reverseUpDir, actor);
    reverseUpDir.negate();

    f32 distance = sead::Mathf::clamp((target - al::getTrans(actor)).dot(reverseUpDir), 500.0f,
                                      riseDistance + 500.0f);

    *out = al::getTrans(actor) + distance * reverseUpDir;
}

}  // namespace

Utsubo::Utsubo(const char* name, bool isWaitForWatcher)
    : al::LiveActor(name), mIsWaitForWatcher(isWaitForWatcher) {}

inline bool Utsubo::isAttackSensorActive() const {
    return al::isNerve(this, &NrvUtsubo.Rise) || al::isNerve(this, &NrvUtsubo.AttackSign) ||
           al::isNerve(this, &NrvUtsubo.Attack) || al::isNerve(this, &NrvUtsubo.Sink);
}

inline void Utsubo::updateBodyAndCapSensorPos(f32 riseDistance) {
    sead::Vector3f sensorPos = sead::Vector3f::zero;
    calcBodySensorPos(&sensorPos, this, rs::getPlayerPos(al::getPlayerActor(this, 0)),
                      riseDistance);
    mBodySensorPos = sensorPos;
    sead::Vector3f capPos = sead::Vector3f::zero;
    if (rs::tryGetFlyingCapPos(&capPos, this)) {
        sead::Vector3f* bodyCapSensorPos = &mBodyCapSensorPos;
        calcBodySensorPos(&sensorPos, this, capPos, riseDistance);
        bodyCapSensorPos->set(sensorPos);
    }
}

void Utsubo::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "Utsubo", nullptr);
    al::hideModel(this);
    al::setHitSensorPosPtr(this, "Body", &mBodySensorPos);
    al::setHitSensorPosPtr(this, "BodyCap", &mBodyCapSensorPos);

    al::tryGetArg(&mIsOnDepthShadow, info, "IsOnDepthShadow");
    if (mIsOnDepthShadow)
        al::onDepthShadowModel(this);
    else
        al::offDepthShadowModel(this);

    mMoveAreaGroup = al::createLinkAreaGroup(this, info, "UtsuboMoveArea",
                                             "ウツボー移動エリアグループ", "子供エリア");
    if (!UtsuboFunction::isMove(mMoveAreaGroup))
        al::offCollide(this);

    if (mIsWaitForWatcher)
        al::initNerve(this, &NrvUtsubo.WaitForWatcher, 0);
    else
        al::initNerve(this, &NrvUtsubo.Wait, 0);

    f32 riseMax = 0.0f;
    if (al::tryGetArg(&riseMax, info, "RiseMax"))
        mRiseMax = riseMax;

    if (!isUpright(this))
        al::invalidateShadow(this);

    al::tryGetArg(&mIsCloudSeaPlacement, info, "IsCloudSeaPlacement");
    al::trySetEffectNamedMtxPtr(this, "Surface", &mSurfaceMtx);
    if (mIsCloudSeaPlacement)
        al::setMaterialCode(this, "Cloud");

    bool hasShine = al::isExistLinkChild(info, "Shine", 0);
    bool hasShineChip = al::isExistLinkChild(info, "ShineChip", 0);
    if (hasShine != hasShineChip) {
        al::PlacementInfo placementInfo;
        const char* linkName = hasShine ? "Shine" : "ShineChip";
        al::getLinksInfoByIndex(&placementInfo, info, linkName, 0);
        if (al::isEqualString(linkName, "Shine")) {
            mLinkedShineActor = rs::initLinkShopShine(info, "Shine");
            mLinkedShineActor->makeActorAlive();
            makeActorAlive();
            return;
        }

        if (al::isEqualString(linkName, "ShineChip")) {
            auto* shineChip = new ShineChip("ウツボー装飾(シャインチップ)", false, false);
            mLinkedShineActor = shineChip;
            al::initCreateActorWithPlacementInfo(shineChip, info, placementInfo);
            mLinkedShineActor->makeActorAlive();
            makeActorAlive();
            return;
        }
    }

    makeActorAlive();
}

void Utsubo::initAfterPlacement() {
    al::LiveActor::initAfterPlacement();
    al::updateMaterialCodeWater(this);
}

void Utsubo::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isSensorEnemyAttack(self) && isAttackSensorActive()) {
        rs::sendMsgUtsuboAttack(other, self);
        rs::sendMsgEnemyObjBreak(other, self);
        if (!al::sendMsgEnemyAttack(other, self))
            rs::sendMsgPushToPlayer(other, self);
    }
}

bool Utsubo::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message))
        return true;

    if (rs::isMsgYoshiTongueAttack(message) ||
        (rs::isMsgCapReflect(message) && al::isSensorEnemyBody(self) && isAttack())) {
        rs::requestHitReactionToAttacker(message, self, other);
        return true;
    }

    return false;
}

bool Utsubo::isAttack() const {
    return al::isNerve(this, &NrvUtsubo.RiseSign) || isAttackSensorActive();
}

void Utsubo::control() {
    if (mLinkedShineActor)
        al::multVecPose(al::getTransPtr(mLinkedShineActor), this, cLinkedShineLocalOffset);

    al::makeMtxQuatPos(&mSurfaceMtx, al::getQuat(this), mRiseStartTrans);
    mPrevPlayerPos = rs::getPlayerPos(al::getPlayerActor(this, 0));
}

void Utsubo::exeWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        al::validateClipping(this);
    }

    if (UtsuboFunction::isInSerchRange(this, mPrevPlayerPos, UtsuboFunction::isMove(mMoveAreaGroup),
                                       cAttackParam)) {
        setNerveRiseSign();
        return;
    }

    if (!UtsuboFunction::isMove(mMoveAreaGroup))
        return;

    if (!al::isOnGround(this, 0))
        al::addVelocityToGravity(this, 1.0f);

    if (al::isGreaterEqualStep(this, 120)) {
        al::setNerve(this, &NrvUtsubo.Move);
        return;
    }

    sead::Vector3f playerPosAfterMove = sead::Vector3f::zero;
    UtsuboFunction::calcPlayerPosAfterMove(&playerPosAfterMove, this, mPrevPlayerPos, cAttackParam);
    if (UtsuboFunction::isMove(mMoveAreaGroup)) {
        if ((al::getTrans(this) - playerPosAfterMove).length() <= 3600.0f)
            al::setNerve(this, &NrvUtsubo.Follow);
    }
}

void Utsubo::exeWaitForWatcher() {}

void Utsubo::exeMove() {
    if (al::isFirstStep(this)) {
        sead::Vector3f frontDir;
        al::calcFrontDir(&frontDir, this);
        al::rotateVectorDegreeY(&frontDir, al::getRandom(360));
        al::setVelocity(this, frontDir * 6.5f);
    }

    if (!al::isOnGround(this, 0))
        al::addVelocityToGravity(this, 1.0f);

    if (!al::isInAreaObj(mMoveAreaGroup, al::getTrans(this) + al::getVelocity(this)) ||
        al::isGreaterEqualStep(this, 120)) {
        al::setVelocityZero(this);
        al::setNerve(this, &NrvUtsubo.Wait);
        return;
    }

    sead::Vector3f playerPosAfterMove = sead::Vector3f::zero;
    UtsuboFunction::calcPlayerPosAfterMove(&playerPosAfterMove, this, mPrevPlayerPos, cAttackParam);
    if ((al::getTrans(this) - playerPosAfterMove).length() <= 3600.0f) {
        al::setNerve(this, &NrvUtsubo.Follow);
        return;
    }

    if (UtsuboFunction::isInSerchRange(this, mPrevPlayerPos, UtsuboFunction::isMove(mMoveAreaGroup),
                                       cAttackParam))
        setNerveRiseSign();
}

void Utsubo::exeFollow() {
    sead::Vector3f playerPosAfterMove = sead::Vector3f::zero;
    UtsuboFunction::calcPlayerPosAfterMove(&playerPosAfterMove, this, mPrevPlayerPos, cAttackParam);
    sead::Vector3f moveDir = playerPosAfterMove - al::getTrans(this);
    moveDir.y = 0.0f;
    al::tryNormalizeOrDirZ(&moveDir);
    al::setVelocity(this, moveDir * 150.0f);

    if (!al::isOnGround(this, 0))
        al::addVelocityToGravity(this, 1.0f);

    if (!al::isInAreaObj(mMoveAreaGroup, al::getTrans(this) + al::getVelocity(this)) ||
        al::isGreaterEqualStep(this, 180)) {
        al::setVelocityZero(this);
        al::setNerve(this, &NrvUtsubo.WaitForce);
        return;
    }

    if (UtsuboFunction::isInSerchRange(this, mPrevPlayerPos, UtsuboFunction::isMove(mMoveAreaGroup),
                                       cAttackParam))
        setNerveRiseSign();
}

void Utsubo::exeRiseSign() {
    if (al::isFirstStep(this)) {
        al::invalidateClipping(this);
        al::setVelocityZero(this);
        mRiseStartTrans.set(al::getTrans(this));
        al::showModelIfHide(this);

        if (isUpright(this)) {
            al::validateShadow(this);
            al::setShadowMaskIntensity(this, "シャドウマスク", 1.0f);
        }

        al::startAction(this, isUpright(this) ? "RiseSignUp" : "RiseSignSide");
    }

    if (isUpright(this))
        al::setShadowMaskIntensity(this, "シャドウマスク", 1.0f - al::calcNerveRate(this, 30));

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvUtsubo.Rise);
}

void Utsubo::exeRise() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Rise");
        mRiseStartTrans = al::getTrans(this);
    }

    sead::Vector3f upDir;
    al::calcUpDir(&upDir, this);

    f32 riseStepDistance = al::getNerveStep(this) * 30.0f;
    f32 riseDistance = sead::Mathf::min(mRiseMax, riseStepDistance);

    al::setTrans(this, upDir * riseDistance + mRiseStartTrans);

    updateBodyAndCapSensorPos(riseDistance);

    if (mRiseMax <= riseDistance)
        al::setNerve(this, &NrvUtsubo.AttackSign);
}

void Utsubo::exeAttackSign() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "AttackSign");
        al::invalidateShadow(this);
    }

    updateBodyAndCapSensorPos(mRiseMax);

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvUtsubo.Attack);
}

void Utsubo::exeAttack() {
    if (al::isFirstStep(this)) {
        al::showModelIfHide(this);
        al::startAction(this, "Attack");
    }

    updateBodyAndCapSensorPos(mRiseMax);

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvUtsubo.Sink);
}

void Utsubo::exeSink() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Sink");
        al::offCollide(this);
    }

    sead::Vector3f upDir;
    al::calcUpDir(&upDir, this);

    s32 sinkStep = mRiseMax / 25.0f;
    f32 riseDistance = al::calcNerveSquareInValue(this, sinkStep, mRiseMax, 0.0f);
    al::setTrans(this, riseDistance * upDir + mRiseStartTrans);

    updateBodyAndCapSensorPos(riseDistance);

    if (al::isGreaterEqualStep(this, sinkStep)) {
        al::hideModelIfShow(this);
        if (UtsuboFunction::isMove(mMoveAreaGroup))
            al::onCollide(this);
        al::setNerve(this, &NrvUtsubo.WaitForce);
    }
}

void Utsubo::exeWaitForce() {
    if (al::isFirstStep(this))
        al::setVelocityZero(this);

    if (al::isGreaterEqualStep(this, 120)) {
        if (mIsWaitForWatcher)
            al::setNerve(this, &NrvUtsubo.WaitForWatcher);
        else if (UtsuboFunction::isMove(mMoveAreaGroup))
            al::setNerve(this, &NrvUtsubo.Move);
        else
            al::setNerve(this, &NrvUtsubo.Wait);
    }
}

bool Utsubo::isRiseReady() const {
    return al::isNerve(this, &NrvUtsubo.WaitForWatcher);
}

void Utsubo::setNerveRiseSign() {
    al::setNerve(this, &NrvUtsubo.RiseSign);
}
