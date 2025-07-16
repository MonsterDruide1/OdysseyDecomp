#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

#include "Player/IUsePlayerCollision.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Se/SeFunction.h"
#include "Library/Shadow/ActorShadowUtil.h"

#include "Enemy/EnemyStateHackStart.h"
#include "Enemy/HackerDepthShadowMapCtrl.h"
#include "Player/CollisionShapeInfo.h"
#include "Player/CollisionShapeKeeper.h"
#include "Player/HackerJudge.h"
#include "Player/HackerStateWingFly.h"
#include "Player/PlayerActionTurnControl.h"
#include "Player/PlayerCollider.h"
#include "Util/Hack.h"
#include "Util/JudgeUtil.h"
#include "Util/PlayerCollisionUtil.h"
#include "Util/SensorMsgFunction.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

class CollisionShapeKeeper;
class HackerDepthShadowMapCtrl;
class HackerJudgeNormalFall;
class HackerJudgeStartRun;
class HackerStateWingFly;
class IUsePlayerHack;
class PlayerActionTurnControl;
class PlayerCollider;

// Inline from math util??
static void makeQuatXDegree(sead::Quatf* outQuat, f32 angle) {
    f32 angleRad = sead::Mathf::deg2rad(angle) * 0.5f;
    f32 cos = sead::Mathf::cos(angleRad);
    f32 sin = sead::Mathf::sin(angleRad);
    outQuat->w = cos;
    outQuat->x = sin;
    outQuat->y = 0.0f;
    outQuat->z = 0.0f;
}

static void maka(CollisionShapeKeeper* mCollisionShapeKeeper, f32 fa, f32 fb,const sead::Vector3f& unk){
    sead::Vector3f vva = fa * sead::Vector3f::ez;
    sead::Vector3f vvb = fb* sead::Vector3f::ey;
    const char* bodyParts[] = {"LegFront", "LegLeft", "LegRight"};
    for (s32 i = 0; i < 3; i++) {
        sead::Quatf quat;
        makeQuatXDegree(&quat, i * 120.0f);

        sead::Vector3f va = fa * sead::Vector3f::ey;
        va.rotate(quat);
        va=va+ unk-vvb;

        sead::Vector3f vb = vvb - 20.0f * sead::Vector3f::ey;
        mCollisionShapeKeeper->createShapeArrow(bodyParts[i], va,vb,
                                                20.0f, i);
    }

    mCollisionShapeKeeper->updateShape();
}

class KaronWingStateHack : public al::ActorStateBase, public IUsePlayerCollision {
public:
    KaronWingStateHack(al::LiveActor* parent, const al::ActorInitInfo& info,
                       IUsePlayerHack** playerHack);

    void appear() override;
    void kill() override;
    void control() override;
    PlayerCollider* getPlayerCollider() const override;

    void attackSensor(al::HitSensor* self, al::HitSensor* other);
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
    void updateCollider();
    void resetFlyLimit(const sead::Vector3f& flyLimit);
    bool isEndCancel() const;
    bool isEndReset() const;
    bool isEndDamage() const;
    void updateBasePos();
    bool tryFly();
    bool tryLanding();

    void exeWait();
    void exeWalk();
    void exeLand();
    void exeWingFly();

    void exeEndCancel() {}

    void exeEndReset() {}

    void exeEndDamage() {}

private:
    IUsePlayerHack** mPlayerHack = nullptr;
    PlayerActionTurnControl* mPlayerActionTurnControl = nullptr;
    PlayerCollider* mPlayerCollider = nullptr;
    CollisionShapeKeeper* mCollisionShapeKeeper = nullptr;
    HackerJudgeNormalFall* mJudgeNormalFall = nullptr;
    HackerJudgeStartRun* mJudgeStartRun = nullptr;
    HackerStateWingFly* mStateWingFly = nullptr;
    f32 mShadowMaskDropLength = 0.0f;
    HackerDepthShadowMapCtrl* mDepthShadowMapCtrl = nullptr;
};
