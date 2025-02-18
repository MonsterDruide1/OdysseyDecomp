#include "Library/LiveActor/ActorActionFunction.h"

#include "Library/Action/ActorActionKeeper.h"
#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/HitReactionKeeper.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveAction.h"
#include "Library/Nerve/NerveActionCtrl.h"
#include "Library/Nerve/NerveKeeper.h"
#include "Library/Nerve/NerveUtil.h"
#include "Project/Action/ActionAnimCtrl.h"
#include "Project/Action/ActionSeCtrl.h"

namespace al {

void startAction(LiveActor* actor, const char* actionName) {
    if (!actor->getActorActionKeeper() || !actor->getActorActionKeeper()->startAction(actionName)) {
        tryStartSklAnimIfExist(actor, actionName);
        tryStartMtpAnimIfExist(actor, actionName);
        tryStartMclAnimIfExist(actor, actionName);
        tryStartMtsAnimIfExist(actor, actionName);
        tryStartVisAnimIfExistForAction(actor, actionName);
    }
}

s32 startActionAtRandomFrame(LiveActor* actor, const char* actionName) {
    startAction(actor, actionName);
    s32 frame = getRandom(0.0f, getSklAnimFrameMax(actor, 0));
    setSklAnimFrame(actor, frame, 0);
    return frame;
}

bool tryStartAction(LiveActor* actor, const char* actionName) {
    if (actor->getActorActionKeeper() && actor->getActorActionKeeper()->getAnimCtrl() &&
        actor->getActorActionKeeper()->getAnimCtrl()->isExistAction(actionName)) {
        actor->getActorActionKeeper()->startAction(actionName);
        return true;
    }

    bool sklStarted = tryStartSklAnimIfExist(actor, actionName);
    bool mtpStarted = tryStartMtpAnimIfExist(actor, actionName);
    bool mclStarted = tryStartMclAnimIfExist(actor, actionName);
    bool mtsStarted = tryStartMtsAnimIfExist(actor, actionName);
    bool visStarted = tryStartVisAnimIfExistForAction(actor, actionName);

    if (!sklStarted && !mtpStarted && !mclStarted && !mtsStarted && !visStarted)
        return false;
    if (actor->getActorActionKeeper())
        actor->getActorActionKeeper()->startAction(actionName);
    return true;
}

bool tryStartActionIfNotPlaying(LiveActor* actor, const char* actionName) {
    if (isActionPlaying(actor, actionName))
        return false;

    startAction(actor, actionName);
    return true;
}

bool isActionPlaying(const LiveActor* actor, const char* actionName) {
    const char* playingActionName = nullptr;
    ActorActionKeeper* keeper = actor->getActorActionKeeper();
    if (keeper && keeper->getAnimCtrl())
        playingActionName = keeper->getAnimCtrl()->getPlayingActionName();
    if (!playingActionName)
        playingActionName = alAnimFunction::getAllAnimName(actor);
    return playingActionName && isEqualString(playingActionName, actionName);
}

bool tryStartActionIfActionEnd(LiveActor* actor, const char* actionName) {
    if (isActionPlaying(actor, actionName) && !isActionEnd(actor))
        return false;

    startAction(actor, actionName);
    return true;
}

bool isActionEnd(const LiveActor* actor) {
    const ActorActionKeeper* keeper = actor->getActorActionKeeper();
    if (keeper && keeper->getAnimCtrl())
        return keeper->getAnimCtrl()->isActionEnd();
    return alAnimFunction::isAllAnimEnd(actor, -1);
}

bool isExistAction(const LiveActor* actor) {
    return isSklAnimExist(actor) || isMtpAnimExist(actor) || isMclAnimExist(actor) ||
           isMtsAnimExist(actor) || isVisAnimExist(actor);
}

bool isExistAction(const LiveActor* actor, const char* actionName) {
    const ActorActionKeeper* keeper = actor->getActorActionKeeper();
    if (keeper && keeper->getAnimCtrl() && keeper->getAnimCtrl()->isExistAction(actionName))
        return true;
    return isSklAnimExist(actor, actionName) || isMtpAnimExist(actor, actionName) ||
           isMclAnimExist(actor, actionName) || isMtsAnimExist(actor, actionName) ||
           isVisAnimExist(actor, actionName);
}

bool isActionOneTime(const LiveActor* actor) {
    return isActionOneTime(actor, getActionName(actor));
}

bool isActionOneTime(const LiveActor* actor, const char* actionName) {
    if (actor->getActorActionKeeper() && actor->getActorActionKeeper()->getAnimCtrl() &&
        actor->getActorActionKeeper()->getAnimCtrl()->isExistAction(actionName))
        return actor->getActorActionKeeper()->getAnimCtrl()->isActionOneTime(actionName);

    if (isSklAnimExist(actor, actionName))
        return isSklAnimOneTime(actor, actionName);
    if (isMtpAnimExist(actor, actionName))
        return isMtpAnimOneTime(actor, actionName);
    if (isMclAnimExist(actor, actionName))
        return isMclAnimOneTime(actor, actionName);
    if (isMtsAnimExist(actor, actionName))
        return isMtsAnimOneTime(actor, actionName);
    if (isVisAnimExist(actor, actionName))
        return isVisAnimOneTime(actor, actionName);
    return false;
}

const char* getActionName(const LiveActor* actor) {
    const ActorActionKeeper* keeper = actor->getActorActionKeeper();
    if (keeper && keeper->getAnimCtrl()) {
        const char* actionName = keeper->getAnimCtrl()->getPlayingActionName();
        if (actionName)
            return actionName;
    }
    return alAnimFunction::getAllAnimName(actor);
}

f32 getActionFrame(const LiveActor* actor) {
    const ActorActionKeeper* keeper = actor->getActorActionKeeper();
    if (keeper && keeper->getAnimCtrl())
        return keeper->getAnimCtrl()->getFrame();
    return alAnimFunction::getAllAnimFrame(actor, -1);
}

f32 getActionFrameMax(const LiveActor* actor) {
    return getActionFrameMax(actor, getActionName(actor));
}

f32 getActionFrameMax(const LiveActor* actor, const char* actionName) {
    const ActorActionKeeper* keeper = actor->getActorActionKeeper();
    if (keeper && keeper->getAnimCtrl())
        return keeper->getAnimCtrl()->getActionFrameMax(actionName);
    return alAnimFunction::getAllAnimFrameMax(actor, actionName, -1);
}

f32 getActionFrameRate(const LiveActor* actor) {
    const ActorActionKeeper* keeper = actor->getActorActionKeeper();
    if (keeper && keeper->getAnimCtrl())
        return keeper->getAnimCtrl()->getFrameRate();
    return alAnimFunction::getAllAnimFrameRate(actor, -1);
}

void setActionFrame(LiveActor* actor, f32 frame) {
    if (isSklAnimExist(actor))
        setSklAnimFrame(actor, frame, 0);
}

void setActionFrameRate(LiveActor* actor, f32 rate) {
    if (isSklAnimExist(actor))
        setSklAnimFrameRate(actor, rate, 0);
}

void stopAction(LiveActor* actor) {
    if (isSklAnimExist(actor))
        setSklAnimFrameRate(actor, 0.0f, 0);
    if (isMtpAnimExist(actor))
        setMtpAnimFrameRate(actor, 0.0f);
    if (isMclAnimExist(actor))
        setMclAnimFrameRate(actor, 0.0f);
    if (isMtsAnimExist(actor))
        setMtsAnimFrameRate(actor, 0.0f);
    if (isVisAnimExist(actor) && isVisAnimPlayingForAction(actor))
        setVisAnimFrameRateForAction(actor, 0.0f);

    if (actor->getActorActionKeeper() && actor->getActorActionKeeper()->getSeCtrl())
        actor->getActorActionKeeper()->getSeCtrl()->resetAction(true);
}

void restartAction(LiveActor* actor) {
    if (isSklAnimExist(actor))
        setSklAnimFrameRate(actor, 1.0f, 0);
    if (isMtpAnimExist(actor))
        setMtpAnimFrameRate(actor, 1.0f);
    if (isMclAnimExist(actor))
        setMclAnimFrameRate(actor, 1.0f);
    if (isMtsAnimExist(actor))
        setMtsAnimFrameRate(actor, 1.0f);
    if (isVisAnimExist(actor) && isVisAnimPlayingForAction(actor))
        setVisAnimFrameRateForAction(actor, 1.0f);

    if (actor->getActorActionKeeper() && actor->getActorActionKeeper()->getSeCtrl())
        actor->getActorActionKeeper()->getSeCtrl()->restartAction();
}

void copyAction(LiveActor* actor, const LiveActor* sourceActor) {
    startAction(actor, getActionName(sourceActor));
    if (isSklAnimExist(sourceActor) && isSklAnimExist(actor))
        copySklAnim(actor, sourceActor);
}

void startNerveAction(LiveActor* actor, const char* actionName) {
    if (actor->getActorActionKeeper())
        actor->getActorActionKeeper()->tryStartActionNoAnim(actionName);
    alNerveFunction::setNerveAction(actor, actionName);
}

void setNerveAtActionEnd(LiveActor* actor, const Nerve* nerve) {
    if (isActionEnd(actor))
        setNerve(actor, nerve);
}

void resetNerveActionForInit(LiveActor* actor) {
    NerveActionCtrl* actionCtrl = actor->getNerveKeeper()->getActionCtrl();
    const Nerve* currentNerve = actor->getNerveKeeper()->getCurrentNerve();
    if (actionCtrl->getNumActions() <= 0)
        return;

    s32 index = 0;
    ;
    for (index = 0; index < actionCtrl->getNumActions(); index++) {
        if (actionCtrl->getAction(index) == currentNerve) {
            startNerveAction(actor, actionCtrl->getAction(index)->getActionName());
            break;
        }
    }
}

void startHitReaction(const LiveActor* actor, const char* name) {
    if (actor->getHitReactionKeeper())
        actor->getHitReactionKeeper()->start(name, nullptr, nullptr, nullptr);
}

void startHitReactionHitEffect(const LiveActor* actor, const char* name, const HitSensor* other,
                               const HitSensor* self) {
    if (actor->getHitReactionKeeper())
        actor->getHitReactionKeeper()->start(name, nullptr, other, self);
}

void startHitReactionHitEffect(const LiveActor* actor, const char* name,
                               const sead::Vector3f& pos) {
    if (actor->getHitReactionKeeper())
        actor->getHitReactionKeeper()->start(name, &pos, nullptr, nullptr);
}

void startHitReactionHitEffect(const LiveActor* actor, const char* name,
                               const sead::Matrix34f* mtx) {
    if (actor->getHitReactionKeeper()) {
        sead::Vector3f pos;
        mtx->getBase(pos, 3);

        // copying around again to force more compact stp codegen
        sead::Vector3f x;
        x.set(pos);
        actor->getHitReactionKeeper()->start(name, &x, nullptr, nullptr);
    }
}

void startHitReactionBlowHit(const LiveActor* actor, const HitSensor* other,
                             const HitSensor* self) {
    startHitReactionHitEffect(actor, "吹き飛びヒット", other, self);
}

void startHitReactionBlowHit(const LiveActor* actor, const sead::Vector3f& pos) {
    startHitReactionHitEffect(actor, "吹き飛びヒット", pos);
}

void startHitReactionBlowHit(const LiveActor* actor) {
    startHitReaction(actor, "吹き飛びヒット");
}

void startHitReactionBlowHitDirect(const LiveActor* actor, const HitSensor* other,
                                   const HitSensor* self) {
    startHitReactionHitEffect(actor, "吹き飛びヒット[直接ヒット]", other, self);
}

void startHitReactionBlowHitDirect(const LiveActor* actor, const sead::Vector3f& pos) {
    startHitReactionHitEffect(actor, "吹き飛びヒット[直接ヒット]", pos);
}

void startHitReactionBlowHitDirect(const LiveActor* actor) {
    startHitReaction(actor, "吹き飛びヒット[直接ヒット]");
}

}  // namespace al
