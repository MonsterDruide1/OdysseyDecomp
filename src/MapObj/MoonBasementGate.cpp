#include "MapObj/MoonBasementGate.h"

#include <math/seadMatrix.h>

#include "Library/Base/StringUtil.h"
#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/BreakModel.h"
#include "Library/Obj/CollisionObj.h"
#include "Library/Obj/PartsFunction.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Boss/Koopa/KoopaHackStopCtrl.h"
#include "Npc/PeachOnKoopaAnimRequester.h"
#include "Util/SensorMsgFunction.h"

namespace alSeFunction {
void startSituation(const al::IUseAudioKeeper* user, const char* situationName, s32 fadeFrames);
void endSituation(const al::IUseAudioKeeper* user, const char* situationName, s32 fadeFrames);
}  // namespace alSeFunction

namespace {
NERVE_IMPL(MoonBasementGate, Invalid)
NERVE_IMPL(MoonBasementGate, BreakAfter)
NERVE_IMPL(MoonBasementGate, Wait)
NERVE_IMPL(MoonBasementGate, Break)
NERVE_IMPL(MoonBasementGate, ClimaxCamera)

NERVES_MAKE_NOSTRUCT(MoonBasementGate, Invalid, BreakAfter)
NERVES_MAKE_STRUCT(MoonBasementGate, Wait, Break, ClimaxCamera)

const char sPartsSuffixCBA[] = {'C', 'B', 'A', '\0'};
const char sPartsSuffixCB[] = {'C', 'B', '\0'};
const char* sPartsSuffixes[] = {sPartsSuffixCBA, sPartsSuffixCB, "C", "", "D"};
const char* sBreakActions[] = {"Break1", "Break2", "Break3", "Break4", "Break5"};
const char* sBreakReactions[] = {"ひび割れ1", "ひび割れ2", "ひび割れ3", "ひび割れ4", "ひび割れ5"};

void moveSlideToTarget(al::LiveActor* actor, const sead::Vector3f& targetTrans) {
    sead::Vector3f direction = targetTrans - al::getTrans(actor);
    al::verticalizeVec(&direction, al::getFront(actor), direction);

    sead::Vector3f nextTrans = al::getTrans(actor) + direction;
    al::resetPosition(actor, nextTrans);
}
}  // namespace

MoonBasementGate::MoonBasementGate(const char* name) : al::LiveActor(name) {}

void MoonBasementGate::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &Invalid, 0);
    al::initActorBgmKeeper(this, info, nullptr);
    al::invalidateHitSensorPlayerAll(this);

    sead::Matrix34f* matrix = new sead::Matrix34f;
    al::makeMtxSRT(matrix, this);

    const char* modelName = al::getModelName(this);
    al::HitSensor* collisionSensor = al::getHitSensor(this, "Collision");
    mCollisionObj =
        al::createCollisionObjMtx(this, info, modelName, collisionSensor, matrix, nullptr);
    mCollisionObj->makeActorAlive();

    if (al::isEqualString(al::getModelName(this), "MoonBasementGate")) {
        mBreakModel = al::createBreakModel(this, info, "崩落ステージのゲート壊れモデル",
                                           "MoonBasementGateBreak", nullptr, nullptr, "Break");
    } else if (al::isEqualString(al::getModelName(this), "MoonBasementGateHard")) {
        mBreakModel = al::createBreakModel(this, info, "崩落ステージのゲート壊れモデル",
                                           "MoonBasementGateHardBreak", nullptr, nullptr, "Break");
    }

    al::initSubActorKeeperNoFile(this, info, 5);
    mParts = new MoonBasementGateParts*[5];

    IMoonBasementGateParts* firstPrevParts = this;
    s32 prevIndex = -1;
    const char* breakModelFormat = "MoonBasementGateParts%sBreak";
    for (s32 i = 0; i < 5; i++) {
        s32 next = i + 1;
        sead::Vector3f partsTrans(0.0f, 0.0f, 0.0f);
        sead::Vector3f localTrans(0.0f, 0.0f, next * 400.0f);
        al::multVecPose(&partsTrans, this, localTrans);

        mParts[i] = new MoonBasementGateParts("");

        const char* suffix = sPartsSuffixes[i];
        MoonBasementGateParts* parts = mParts[i];
        al::StringTmp<64> model("MoonBasementGateParts%s", suffix);
        const char* partsModelName = model.cstr();
        al::StringTmp<64> breakModel(breakModelFormat, suffix);

        parts->initParts(info, partsModelName, breakModel.cstr(), partsTrans, next < 5);

        MoonBasementGateParts* currentParts = mParts[i];
        IMoonBasementGateParts* prevParts;
        if (i < 1) {
            prevParts = firstPrevParts;
        } else {
            MoonBasementGateParts* prevActor = mParts[prevIndex];
            prevParts = prevActor != nullptr ? prevActor : nullptr;
        }
        currentParts->setPrevParts(prevParts);

        al::registerSubActorSyncClipping(this, mParts[i]);
        prevIndex++;
    }

    if (al::isExistLinkChild(info, "ClimaxEventKeyMoveCamera", 0)) {
        mClimaxCameraTicket = al::initAnimCamera(this, info, "Anim");
        al::tryGetLinksQT(&mClimaxAfterKoopaQuat, &mClimaxAfterKoopaTrans, info,
                          "ClimaxAfterKoopaPosture");
    }

    makeActorAlive();
}

void MoonBasementGate::kill() {
    al::LiveActor::kill();
    mCollisionObj->kill();

    for (s32 i = 0; i < 5; i++)
        if (al::isAlive(mParts[i]))
            mParts[i]->kill();

    al::tryOnSwitchDeadOn(this);
    al::tryOffStageSwitch(this, "SwitchAllRockBreakOn");
}

void MoonBasementGate::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isSensorPlayerAttack(self))
        rs::sendMsgMoonBasementBreakShockwaveMeteor(other, self);
}

bool MoonBasementGate::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                  al::HitSensor* self) {
    if (rs::isMsgKoopaInvalidHackPunchFaceToCollision(message))
        return true;

    bool canReceive = al::isNerve(this, &NrvMoonBasementGate.Wait);
    if (!canReceive && al::isNerve(this, &NrvMoonBasementGate.Break))
        canReceive = al::isGreaterStep(this, 12);

    if (!canReceive)
        return false;

    bool isPunchCollide = rs::isMsgKoopaHackPunchCollide(message);
    if (!isPunchCollide) {
        if (al::isEqualString(al::getModelName(this), "MoonBasementGateHard"))
            return false;
        if (!al::isMsgExplosionCollide(message))
            return false;
    }

    if (mBreakCount <= 5 && al::isExistAction(this, sBreakActions[mBreakCount])) {
        al::setNerve(this, &NrvMoonBasementGate.Break);
        return true;
    }

    al::startHitReaction(this, "破壊");

    if (mBreakModel != nullptr)
        mBreakModel->appear();

    if (mClimaxCameraTicket == nullptr) {
        rs::requestStartPeachOnKoopaAnimSuccess(this);
        kill();
    } else {
        al::tryOnStageSwitch(this, "SwitchClimaxCameraStartOn");
        al::setNerve(this, &NrvMoonBasementGate.ClimaxCamera);
    }

    return true;
}

void MoonBasementGate::validate() {
    al::setNerve(this, &NrvMoonBasementGate.Wait);
}

void MoonBasementGate::moveSlideToTarget(const sead::Vector3f& targetTrans) {
    if (al::isEqualString(al::getModelName(this), "MoonBasementGateHard")) {
        al::tryOnStageSwitch(this, "SwitchAllRockBreakOn");
        return;
    }

    ::moveSlideToTarget(this, targetTrans);
}

void MoonBasementGate::exeInvalid() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void MoonBasementGate::exeWait() {
    if (al::isFirstStep(this))
        al::tryStartActionIfNotPlaying(this, "Wait");
}

void MoonBasementGate::exeBreak() {
    if (al::isFirstStep(this)) {
        al::startAction(this, sBreakActions[mBreakCount]);

        if (al::isEqualString(al::getModelName(this), "MoonBasementGateHard"))
            rs::requestStartPeachOnKoopaHitReactionCrackGateHard(this);

        mBreakCount++;
    }

    if (al::isGreaterEqualStep(this, 1)) {
        if (al::isStep(this, 1))
            al::startHitReaction(this, sBreakReactions[mBreakCount - 1]);

        al::setNerveAtActionEnd(this, &NrvMoonBasementGate.Wait);
    }
}

void MoonBasementGate::exeBreakAfter() {
    if (al::isFirstStep(this)) {
        KoopaHackFunction::startStopKoopaHack(this);
        KoopaHackFunction::setStatusDemoForSceneKoopaHack(this);
        rs::requestStartPeachOnKoopaHitReactionBreakGateHard(this);
        al::hideModelIfShow(this);
        al::validateHitSensor(this, "Shockwave");
        al::stopAllBgm(this, -1);
        alSeFunction::startSituation(this, "崩落ステージ地球デモ", -1);
    }

    if (al::isGreaterEqualStep(this, 60)) {
        al::invalidateHitSensor(this, "Shockwave");
        al::setNerve(this, &BreakAfter);
    }
}

void MoonBasementGate::exeClimaxCamera() {
    if (al::isFirstStep(this)) {
        al::invalidateClipping(this);
        al::startAnimCamera(this, mClimaxCameraTicket, "ClimaxCamera", 0);
    }

    if (al::isStep(this, 1))
        KoopaHackFunction::resetPostureKoopaHack(this, mClimaxAfterKoopaQuat,
                                                 mClimaxAfterKoopaTrans);

    if (al::isStep(this, 170))
        al::startBgm(this, "Climax", -1, 0);

    if (al::isStep(this, 168))
        al::startHitReaction(this, "クライマックスブラー");

    if (al::isEndAnimCamera(mClimaxCameraTicket)) {
        al::endCamera(this, mClimaxCameraTicket, -1, false);
        al::validateClipping(this);
        KoopaHackFunction::endStopKoopaHack(this);
        KoopaHackFunction::resetStatusDemoForSceneKoopaHack(this);
        rs::requestStartPeachOnKoopaAnimSuccess(this);
        alSeFunction::endSituation(this, "崩落ステージ地球デモ", -1);
        kill();
    }
}
