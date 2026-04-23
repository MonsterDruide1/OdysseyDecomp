#include "MapObj/MoonBasementFloor.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "Boss/Koopa/KoopaHackStopCtrl.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(MoonBasementFloor, Wait)
NERVE_IMPL(MoonBasementFloor, FallSignStart)
NERVE_IMPL_(MoonBasementFloor, FallSignStartByMeteor, FallSignStart)
NERVE_IMPL_(MoonBasementFloor, FallSignStartBySwitch, FallSignStart)
NERVE_IMPL(MoonBasementFloor, FallSignBySwitch)
NERVE_IMPL_(MoonBasementFloor, FallByMeteor, Fall)
NERVE_IMPL(MoonBasementFloor, FallSign)
NERVE_IMPL(MoonBasementFloor, Fall)

NERVES_MAKE_STRUCT(MoonBasementFloor, Wait, FallSignStart, FallSignStartByMeteor,
                   FallSignStartBySwitch, FallSignBySwitch, FallByMeteor, FallSign, Fall)

const f32 sFallGravity = 0.2f;
const f32 sFallScale = 0.95f;
const f32 sFallGravityByMeteor = 0.2f;
const f32 sFallScaleByMeteor = 0.95f;

struct MoonFallParams {
    const f32* gravity;
    const f32* scale;
};

const MoonFallParams sFallParams{&sFallGravity, &sFallScale};
const MoonFallParams sFallParamsByMeteor{&sFallGravityByMeteor, &sFallScaleByMeteor};

inline const MoonFallParams& getMoonFallParams(al::LiveActor* actor) {
    return al::isNerve(actor, &NrvMoonBasementFloor.FallByMeteor) ? sFallParamsByMeteor :
                                                                    sFallParams;
}
}  // namespace

MoonBasementFloor::MoonBasementFloor(const char* name) : al::LiveActor(name) {}

void MoonBasementFloor::init(const al::ActorInitInfo& info) {
    using MoonBasementFloorFunctor =
        al::FunctorV0M<MoonBasementFloor*, void (MoonBasementFloor::*)()>;

    al::initActorChangeModel(this, info);
    al::initNerve(this, &NrvMoonBasementFloor.Wait, 0);

    f32 rotate = sead::Mathf::floor(al::getRandom(0.0f, 3.99f));
    al::addRotateAndRepeatY(this, rotate * 90.0);

    if (al::listenStageSwitchOn(
            this, "SwitchFallStart",
            MoonBasementFloorFunctor(this, &MoonBasementFloor::startFallBySwitch))) {
        al::tryGetArg(&mFallSignStepBySwitch, info, "FallSignStepBySwitch");
    }

    al::trySyncStageSwitchKill(this);
}

void MoonBasementFloor::startFallBySwitch() {
    if (!al::isAlive(this))
        return;

    if (al::isNerve(this, &NrvMoonBasementFloor.Fall))
        return;

    if (al::isNerve(this, &NrvMoonBasementFloor.FallByMeteor))
        return;

    s32 fallSignStepBySwitch = mFallSignStepBySwitch;
    al::invalidateClipping(this);

    if (fallSignStepBySwitch == 0)
        al::setNerve(this, &NrvMoonBasementFloor.FallSignStart);
    else
        al::setNerve(this, &NrvMoonBasementFloor.FallSignStartBySwitch);
}

void MoonBasementFloor::movement() {
    if (!KoopaHackFunction::isStopKoopaHack(this))
        al::LiveActor::movement();
}

bool MoonBasementFloor::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                   al::HitSensor* self) {
    if (!al::isNerve(this, &NrvMoonBasementFloor.Wait))
        return false;

    if (rs::isMsgKoopaTouchFloor(message)) {
        al::startHitReaction(this, "クッパ着地");
        al::invalidateClipping(this);
        al::setNerve(this, &NrvMoonBasementFloor.FallSignStart);
        return true;
    }

    if (rs::isMsgMoonBasementAttackMeteor(message)) {
        al::invalidateClipping(this);
        al::setNerve(this, &NrvMoonBasementFloor.FallSignStartByMeteor);
        return true;
    }

    return false;
}

void MoonBasementFloor::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void MoonBasementFloor::exeFallSignStart() {
    if (al::isFirstStep(this))
        al::startAction(this, "FallSignStart");

    if (al::isActionEnd(this)) {
        if (al::isNerve(this, &NrvMoonBasementFloor.FallSignStartBySwitch))
            al::setNerve(this, &NrvMoonBasementFloor.FallSignBySwitch);
        else if (al::isNerve(this, &NrvMoonBasementFloor.FallSignStartByMeteor))
            al::setNerve(this, &NrvMoonBasementFloor.FallByMeteor);
        else
            al::setNerve(this, &NrvMoonBasementFloor.FallSign);
    }
}

void MoonBasementFloor::exeFallSign() {
    if (al::isFirstStep(this))
        al::startAction(this, "FallSign");

    al::setNerveAtActionEnd(this, &NrvMoonBasementFloor.Fall);
}

void MoonBasementFloor::exeFallSignBySwitch() {
    if (al::isFirstStep(this))
        al::startAction(this, "FallSignBySwitch");

    s32 fallSignStep = mFallSignStepBySwitch;
    if (fallSignStep < 0)
        fallSignStep = 30;

    al::setNerveAtGreaterEqualStep(this, &NrvMoonBasementFloor.Fall, fallSignStep);
}

void MoonBasementFloor::exeFall() {
    if (al::isFirstStep(this))
        al::startAction(this, "Fall");

    al::addVelocityToGravity(this, *getMoonFallParams(this).gravity);
    al::scaleVelocity(this, *getMoonFallParams(this).scale);

    if (al::isGreaterEqualStep(this, 300)) {
        al::startHitReaction(this, "消滅");
        kill();
    }
}
