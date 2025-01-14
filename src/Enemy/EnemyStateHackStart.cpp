#include "Enemy/EnemyStateHackStart.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/SubActorKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Shadow/DepthShadowDrawer.h"

#include "Player/PlayerHackStartShaderCtrl.h"
#include "Util/Hack.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(EnemyStateHackStart, DiveIn);
NERVE_IMPL(EnemyStateHackStart, HackStart);

NERVES_MAKE_NOSTRUCT(EnemyStateHackStart, DiveIn, HackStart);
}  // namespace

EnemyStateHackStartParam::EnemyStateHackStartParam(const char* actionName, const char* visAnimName,
                                                   const char* mtpAnimName, bool hasSubActors,
                                                   bool updateSubActorShadowMap)
    : mActionName(actionName), mVisAnimName(visAnimName), mMtpAnimName(mtpAnimName),
      mHasSubActors(hasSubActors), mUpdateSubActorShadowMap(updateSubActorShadowMap) {}

static EnemyStateHackStartParam sEnemyStateHackStartParam("HackStart", 0, 0, 0, 0);

EnemyStateHackStart::EnemyStateHackStart(al::LiveActor* rootActor,
                                         const EnemyStateHackStartParam* param,
                                         PlayerHackStartShaderParam* shaderParam)
    : al::ActorStateBase("憑依開始", rootActor), mParam(param) {
    if (!param)
        mParam = &sEnemyStateHackStartParam;
    initNerve(&DiveIn, 0);
    mPlayerHackStartShaderCtrl = new PlayerHackStartShaderCtrl(rootActor, shaderParam);
}

IUsePlayerHack* EnemyStateHackStart::tryStart(const al::SensorMsg* msg, al::HitSensor* other,
                                              al::HitSensor* self) {
    if (!rs::isMsgStartHack(msg))
        return nullptr;
    al::setVelocityZero(mActor);
    mHackActor = rs::startHack(self, other, 0);
    rs::startHackStartDemo(mHackActor, mActor);
    al::setNerve(this, &DiveIn);
    return mHackActor;
}

void EnemyStateHackStart::kill() {
    setDead(true);
    if (!mHackActor)
        return;
    rs::endHackStartDemo(mHackActor, mActor);
    mHackActor = nullptr;
}

bool EnemyStateHackStart::isHackStart() const {
    return al::isNerve(this, &HackStart);
}

f32 EnemyStateHackStart::calcHackStartNerveRate() const {
    if (!isHackStart())
        return 0.0f;

    s32 frameMax = al::getActionFrameMax(mActor, mParam->mActionName);
    return al::calcNerveRate(this, frameMax);
}

void EnemyStateHackStart::exeDiveIn() {
    if (!rs::isHackStartDemoEnterMario(mHackActor))
        return;
    if (mParam->mActionName)
        al::setNerve(this, &HackStart);
    else
        kill();
}

void EnemyStateHackStart::exeHackStart() {
    if (al::isFirstStep(this)) {
        mPlayerHackStartShaderCtrl->start();
        al::startAction(mActor, mParam->mActionName);

        if (mParam->mHasSubActors) {
            s32 subActorNum = al::getSubActorNum(mActor);
            for (s32 i = 0; i < subActorNum; i++)
                if (al::isExistAction(al::getSubActor(mActor, i), mParam->mActionName))
                    al::startAction(al::getSubActor(mActor, i), mParam->mActionName);
        }
        if (mParam->mVisAnimName)
            al::startVisAnim(mActor, mParam->mVisAnimName);
        if (mParam->mMtpAnimName)
            al::startMtpAnim(mActor, mParam->mMtpAnimName);
        al::LiveActor* actor = mActor;
        if (al::isExistDepthShadowMapCtrl(actor)) {
            al::invalidateShadow(actor);
            al::offDepthShadowModel(actor);
            al::validateDepthShadowMap(actor);
        }
        if (mParam->mUpdateSubActorShadowMap) {
            s32 subActorNum = al::getSubActorNum(mActor);
            for (s32 i = 0; i < subActorNum; i++) {
                al::LiveActor* subActor = al::getSubActor(mActor, i);
                if (al::isExistDepthShadowMapCtrl(subActor)) {
                    al::invalidateShadow(subActor);
                    al::offDepthShadowModel(subActor);
                    al::validateDepthShadowMap(subActor);
                }
            }
        }
    }
    mPlayerHackStartShaderCtrl->update();
    if (al::isActionEnd(mActor)) {
        mPlayerHackStartShaderCtrl->end();
        kill();
    }
}

namespace EnemyStateHackFunction {
void startHackSwitchShadow(al::LiveActor* actor, const EnemyStateHackStartParam* param) {
    if (al::isExistDepthShadowMapCtrl(actor)) {
        al::invalidateShadow(actor);
        al::offDepthShadowModel(actor);
        al::validateDepthShadowMap(actor);
    }
    if (param && param->mUpdateSubActorShadowMap) {
        s32 subActorNum = al::getSubActorNum(actor);
        for (s32 i = 0; i < subActorNum; i++) {
            al::LiveActor* subActor = al::getSubActor(actor, i);
            if (al::isExistDepthShadowMapCtrl(subActor)) {
                al::invalidateShadow(subActor);
                al::offDepthShadowModel(subActor);
                al::validateDepthShadowMap(subActor);
            }
        }
    }
}

void endHackSwitchShadow(al::LiveActor* actor, const EnemyStateHackStartParam* param) {
    if (al::isExistDepthShadowMapCtrl(actor)) {
        al::validateShadow(actor);
        al::onDepthShadowModel(actor);
        al::invalidateDepthShadowMap(actor);
    }
    if (param && param->mUpdateSubActorShadowMap) {
        s32 subActorNum = al::getSubActorNum(actor);
        for (s32 i = 0; i < subActorNum; i++) {
            al::LiveActor* subActor = al::getSubActor(actor, i);
            if (al::isExistDepthShadowMapCtrl(subActor)) {
                al::validateShadow(subActor);
                al::onDepthShadowModel(subActor);
                al::invalidateDepthShadowMap(subActor);
            }
        }
    }
}

}  // namespace EnemyStateHackFunction
