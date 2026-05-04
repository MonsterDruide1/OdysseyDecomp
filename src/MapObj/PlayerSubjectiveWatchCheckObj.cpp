#include "MapObj/PlayerSubjectiveWatchCheckObj.h"

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Screen/ScreenFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Util/PlayerUtil.h"

namespace {
NERVE_ACTION_IMPL(PlayerSubjectiveWatchCheckObj, NotSubjective)
NERVE_ACTION_IMPL(PlayerSubjectiveWatchCheckObj, In)
NERVE_ACTION_IMPL(PlayerSubjectiveWatchCheckObj, Out)
NERVE_ACTIONS_MAKE_STRUCT(PlayerSubjectiveWatchCheckObj, NotSubjective, In, Out)

bool isInWatchScreenRange(const al::LiveActor* actor) {
    sead::Vector3f layoutPos = {0.0f, 0.0f, 0.0f};
    const al::IUseCamera* camera = actor;
    const sead::Vector3f& trans = al::getTrans(actor);
    al::calcLayoutPosFromWorldPosWithClampOutRange(&layoutPos, camera, trans, 50.0f, 0);

    if (layoutPos.z > 0.0f)
        return false;

    const f32 posX = layoutPos.x;
    const f32 minX = 320.0f - al::getDisplayWidth() * 0.5f;
    const f32 maxX = al::getDisplayWidth() * 0.5f + -320.0f;
    if (!al::isInRange(posX, minX, maxX))
        return false;

    const f32 posY = layoutPos.y;
    const f32 minY = 180.0f - al::getDisplayHeight() * 0.5f;
    const f32 maxY = al::getDisplayHeight() * 0.5f + -180.0f;
    return al::isInRange(posY, minY, maxY);
}
}  // namespace

PlayerSubjectiveWatchCheckObj::PlayerSubjectiveWatchCheckObj(const char* name)
    : al::LiveActor(name) {}

void PlayerSubjectiveWatchCheckObj::init(const al::ActorInitInfo& info) {
    al::initNerveAction(this, "NotSubjective", &NrvPlayerSubjectiveWatchCheckObj.collector, 0);
    al::initActor(this, info);
    al::trySyncStageSwitchAppearAndKill(this);
}

void PlayerSubjectiveWatchCheckObj::control() {}

void PlayerSubjectiveWatchCheckObj::appear() {
    al::LiveActor::appear();
    if (!al::isNerve(this, NrvPlayerSubjectiveWatchCheckObj.In.data()))
        return;

    if (rs::isPlayerCameraSubjective(this))
        al::startNerveAction(this, "Out");
    else
        al::startNerveAction(this, "NotSubjective");
}

void PlayerSubjectiveWatchCheckObj::kill() {
    al::LiveActor::kill();
}

void PlayerSubjectiveWatchCheckObj::endClipped() {
    al::LiveActor::endClipped();
    if (!al::isNerve(this, NrvPlayerSubjectiveWatchCheckObj.In.data()))
        return;

    if (rs::isPlayerCameraSubjective(this))
        al::startNerveAction(this, "Out");
    else
        al::startNerveAction(this, "NotSubjective");
}

void PlayerSubjectiveWatchCheckObj::exeNotSubjective() {
    if (rs::isPlayerCameraSubjective(this))
        al::startNerveAction(this, "Out");
}

void PlayerSubjectiveWatchCheckObj::exeIn() {
    if (!isInWatchScreenRange(this)) {
        al::startNerveAction(this, "Out");
        return;
    }

    if (!rs::isPlayerCameraSubjective(this)) {
        al::startNerveAction(this, "NotSubjective");
        return;
    }

    if (al::isGreaterEqualStep(this, 120)) {
        al::tryOnStageSwitch(this, "SwitchWatchOn");
        al::startHitReaction(this, "スイッチオン");
        kill();
    }
}

void PlayerSubjectiveWatchCheckObj::exeOut() {
    if (isInWatchScreenRange(this)) {
        al::startNerveAction(this, "In");
        return;
    }

    if (!rs::isPlayerCameraSubjective(this))
        al::startNerveAction(this, "NotSubjective");
}
