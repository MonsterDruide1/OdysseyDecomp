#include "Library/LiveActor/LiveActorFunction.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorInfo.h"
#include "Library/LiveActor/LiveActorKeeper.h"

namespace alSubActorFunction {

void trySyncAlive(al::SubActorKeeper* subActorKeeper) {
    for (s32 i = 0; i < subActorKeeper->getCurActorCount(); i++)
        if (subActorKeeper->getActorInfo(i)->syncType.isSet(al::SubActorSync::cAppear))
            subActorKeeper->getActorInfo(i)->subActor->makeActorAlive();
}

void trySyncDead(al::SubActorKeeper* subActorKeeper) {
    for (s32 i = 0; i < subActorKeeper->getCurActorCount(); i++)
        if (subActorKeeper->getActorInfo(i)->syncType.isSet(al::SubActorSync::cAppear))
            subActorKeeper->getActorInfo(i)->subActor->makeActorDead();
}

void trySyncClippingStart(al::SubActorKeeper* subActorKeeper) {
    for (s32 i = 0; i < subActorKeeper->getCurActorCount(); i++) {
        const al::SubActorInfo* info = subActorKeeper->getActorInfo(i);

        if (info->syncType.isSet(al::SubActorSync::cClipping) && al::isAlive(info->subActor) &&
            !al::isClipped(info->subActor)) {
            info->subActor->startClipped();
        }
    }
}

void trySyncClippingEnd(al::SubActorKeeper* subActorKeeper) {
    for (s32 i = 0; i < subActorKeeper->getCurActorCount(); i++) {
        const al::SubActorInfo* info = subActorKeeper->getActorInfo(i);

        if (info->syncType.isSet(al::SubActorSync::cClipping) && al::isAlive(info->subActor) &&
            al::isClipped(info->subActor)) {
            info->subActor->endClipped();
        }
    }
}

void trySyncShowModel(al::SubActorKeeper* subActorKeeper) {
    for (s32 i = 0; i < subActorKeeper->getCurActorCount(); i++)
        if (subActorKeeper->getActorInfo(i)->syncType.isSet(al::SubActorSync::cHide))
            al::showModelIfHide(subActorKeeper->getActorInfo(i)->subActor);
}

void trySyncHideModel(al::SubActorKeeper* subActorKeeper) {
    for (s32 i = 0; i < subActorKeeper->getCurActorCount(); i++)
        if (subActorKeeper->getActorInfo(i)->syncType.isSet(al::SubActorSync::cHide))
            al::hideModelIfShow(subActorKeeper->getActorInfo(i)->subActor);
}

void trySyncModelAlphaMask(al::SubActorKeeper* subActorKeeper, f32 alpha) {
    for (s32 i = 0; i < subActorKeeper->getCurActorCount(); i++) {
        const al::SubActorInfo* info = subActorKeeper->getActorInfo(i);

        if (info->syncType.isSet(al::SubActorSync::cAlphaMask) && al::isExistModel(info->subActor))
            al::setModelAlphaMask(info->subActor, alpha);
    }
}

al::LiveActor* findSubActor(const al::SubActorKeeper* subActorKeeper, const char* subActorName) {
    for (s32 i = 0; i < subActorKeeper->getCurActorCount(); i++) {
        const al::SubActorInfo* info = subActorKeeper->getActorInfo(i);

        if (al::isEqualString(info->subActor->getName(), subActorName))
            return info->subActor;
    }
    return nullptr;
}

}  // namespace alSubActorFunction

namespace al {
inline SubActorInfo* getSubActorInfo(const LiveActor* actor, s32 index) {
    return actor->getSubActorKeeper()->getActorInfo(index);
}

inline SubActorInfo* getSubActorInfo(const LiveActor* actor, const LiveActor* subActor) {
    for (s32 i = 0; i < getSubActorNum(actor); i++) {
        SubActorInfo* info = getSubActorInfo(actor, i);
        if (info->subActor == subActor)
            return info;
    }
    return nullptr;
}

bool isExistSubActorKeeper(const LiveActor* actor) {
    return actor->getSubActorKeeper() != nullptr;
}

LiveActor* getSubActor(const LiveActor* actor, const char* subActorName) {
    if (isExistSubActorKeeper(actor))
        return alSubActorFunction::findSubActor(actor->getSubActorKeeper(), subActorName);
    return nullptr;
}

LiveActor* tryGetSubActor(const LiveActor* actor, const char* subActorName) {
    return getSubActor(actor, subActorName);
}

LiveActor* getSubActor(const LiveActor* actor, s32 index) {
    return getSubActorInfo(actor, index)->subActor;
}

s32 getSubActorNum(const LiveActor* actor) {
    return actor->getSubActorKeeper()->getCurActorCount();
}

void offSyncClippingSubActor(LiveActor* actor, const LiveActor* subActor) {
    SubActorInfo* info = getSubActorInfo(actor, subActor);
    __asm("");
    info->syncType.unset(SubActorSync::cClipping);
    return;
}

void offSyncClippingSubActor(LiveActor* actor, const char* subActorName) {
    offSyncClippingSubActor(actor, getSubActor(actor, subActorName));
}

void offSyncClippingSubActorAll(LiveActor* actor) {
    const SubActorKeeper* subActorKeeper = actor->getSubActorKeeper();
    for (s32 i = 0; i < subActorKeeper->getCurActorCount(); i++)
        subActorKeeper->getActorInfo(i)->syncType.unset(SubActorSync::cClipping);
}

void onSyncClippingSubActor(LiveActor* actor, const LiveActor* subActor) {
    getSubActorInfo(actor, subActor)->syncType.set(SubActorSync::cClipping);
}

void onSyncClippingSubActor(LiveActor* actor, const char* subActorName) {
    onSyncClippingSubActor(actor, getSubActor(actor, subActorName));
}

void onSyncClippingSubActorAll(LiveActor* actor) {
    const SubActorKeeper* subActorKeeper = actor->getSubActorKeeper();
    for (s32 i = 0; i < subActorKeeper->getCurActorCount(); i++)
        subActorKeeper->getActorInfo(i)->syncType.set(SubActorSync::cClipping);
}

void offSyncAppearSubActor(LiveActor* actor, const LiveActor* subActor) {
    SubActorInfo* info = getSubActorInfo(actor, subActor);
    __asm("");
    info->syncType.unset(SubActorSync::cAppear);
}

void offSyncAppearSubActor(LiveActor* actor, const char* subActorName) {
    offSyncAppearSubActor(actor, getSubActor(actor, subActorName));
}

void offSyncAppearSubActorAll(LiveActor* actor) {
    const SubActorKeeper* subActorKeeper = actor->getSubActorKeeper();
    for (s32 i = 0; i < subActorKeeper->getCurActorCount(); i++)
        subActorKeeper->getActorInfo(i)->syncType.unset(SubActorSync::cAppear);
}

void onSyncAppearSubActor(LiveActor* actor, const LiveActor* subActor) {
    getSubActorInfo(actor, subActor)->syncType.set(SubActorSync::cAppear);
}

void onSyncAppearSubActor(LiveActor* actor, const char* subActorName) {
    onSyncAppearSubActor(actor, getSubActor(actor, subActorName));
}

void onSyncAppearSubActorAll(LiveActor* actor) {
    const SubActorKeeper* subActorKeeper = actor->getSubActorKeeper();
    for (s32 i = 0; i < subActorKeeper->getCurActorCount(); i++)
        subActorKeeper->getActorInfo(i)->syncType.set(SubActorSync::cAppear);
}

void offSyncHideSubActor(LiveActor* actor, const LiveActor* subActor) {
    SubActorInfo* info = getSubActorInfo(actor, subActor);
    __asm("");
    info->syncType.unset(SubActorSync::cHide);
}

void offSyncHideSubActor(LiveActor* actor, const char* subActorName) {
    offSyncHideSubActor(actor, getSubActor(actor, subActorName));
}

void offSyncHideSubActorAll(LiveActor* actor) {
    const SubActorKeeper* subActorKeeper = actor->getSubActorKeeper();
    for (s32 i = 0; i < subActorKeeper->getCurActorCount(); i++)
        subActorKeeper->getActorInfo(i)->syncType.unset(SubActorSync::cHide);
}

void onSyncHideSubActor(LiveActor* actor, const LiveActor* subActor) {
    getSubActorInfo(actor, subActor)->syncType.set(SubActorSync::cHide);
}

void onSyncHideSubActor(LiveActor* actor, const char* subActorName) {
    onSyncHideSubActor(actor, getSubActor(actor, subActorName));
}

void onSyncHideSubActorAll(LiveActor* actor) {
    const SubActorKeeper* subActorKeeper = actor->getSubActorKeeper();
    for (s32 i = 0; i < subActorKeeper->getCurActorCount(); i++)
        subActorKeeper->getActorInfo(i)->syncType.set(SubActorSync::cHide);
}

bool isSyncHideSubActor(const LiveActor* actor, const LiveActor* subActor) {
    return getSubActorInfo(actor, subActor)->syncType.isSet(SubActorSync::cHide);
}

void offSyncAlphaMaskSubActor(LiveActor* actor, const LiveActor* subActor) {
    SubActorInfo* info = getSubActorInfo(actor, subActor);
    __asm("");
    info->syncType.unset(SubActorSync::cAlphaMask);
}

void offSyncAlphaMaskSubActor(LiveActor* actor, const char* subActorName) {
    offSyncAlphaMaskSubActor(actor, getSubActor(actor, subActorName));
}

void offSyncAlphaMaskSubActorAll(LiveActor* actor) {
    const SubActorKeeper* subActorKeeper = actor->getSubActorKeeper();
    for (s32 i = 0; i < subActorKeeper->getCurActorCount(); i++)
        subActorKeeper->getActorInfo(i)->syncType.unset(SubActorSync::cAlphaMask);
}

void onSyncAlphaMaskSubActor(LiveActor* actor, const LiveActor* subActor) {
    getSubActorInfo(actor, subActor)->syncType.set(SubActorSync::cAlphaMask);
}

void onSyncAlphaMaskSubActor(LiveActor* actor, const char* subActorName) {
    onSyncAlphaMaskSubActor(actor, getSubActor(actor, subActorName));
}

void onSyncAlphaMaskSubActorAll(LiveActor* actor) {
    const SubActorKeeper* subActorKeeper = actor->getSubActorKeeper();
    for (s32 i = 0; i < subActorKeeper->getCurActorCount(); i++)
        subActorKeeper->getActorInfo(i)->syncType.set(SubActorSync::cAlphaMask);
}

bool isSyncAlphaMaskSubActor(const LiveActor* actor, const LiveActor* subActor) {
    return getSubActorInfo(actor, subActor)->syncType.isSet(SubActorSync::cAlphaMask);
}

void startActionSubActor(const LiveActor* actor, const char* subActorName, const char* action) {
    startAction(getSubActor(actor, subActorName), action);
}

bool isActionEndSubActor(const LiveActor* actor, const char* subActorName) {
    return isActionEnd(getSubActor(actor, subActorName));
}

bool isActionPlayingSubActor(const LiveActor* actor, const char* subActorName, const char* action) {
    return isActionPlaying(getSubActor(actor, subActorName), action);
}

bool isActionOneTimeSubActor(const LiveActor* actor, const char* subActorName) {
    return isActionOneTime(getSubActor(actor, subActorName));
}

bool isActionOneTimeSubActor(const LiveActor* actor, const char* subActorName, const char* action) {
    return isActionOneTime(getSubActor(actor, subActorName), action);
}

bool tryStartActionSubActorAll(const LiveActor* actor, const char* action) {
    bool isAnyStartAction = false;
    const SubActorKeeper* subActorKeeper = actor->getSubActorKeeper();
    for (s32 i = 0; i < subActorKeeper->getCurActorCount(); i++)
        isAnyStartAction |= tryStartAction(subActorKeeper->getActorInfo(i)->subActor, action);
    return isAnyStartAction;
}

void makeActorDeadSubActorAll(const LiveActor* actor) {
    const SubActorKeeper* subActorKeeper = actor->getSubActorKeeper();
    for (s32 i = 0; i < subActorKeeper->getCurActorCount(); i++) {
        SubActorInfo* info = subActorKeeper->getActorInfo(i);
        info->subActor->makeActorDead();
        if (isExistSubActorKeeper(info->subActor))
            makeActorDeadSubActorAll(info->subActor);
    }
}

void tryValidateCollisionPartsSubActorAll(LiveActor* actor) {
    s32 count = getSubActorNum(actor);
    for (s32 i = 0; i < count; i++) {
        LiveActor* subActor = getSubActor(actor, i);
        if (isExistCollisionParts(subActor))
            validateCollisionParts(subActor);
    }
}

void tryInvalidateCollisionPartsSubActorAll(LiveActor* actor) {
    s32 count = getSubActorNum(actor);
    for (s32 i = 0; i < count; i++) {
        LiveActor* subActor = getSubActor(actor, i);
        if (isExistCollisionParts(subActor))
            invalidateCollisionParts(subActor);
    }
}

}  // namespace al
