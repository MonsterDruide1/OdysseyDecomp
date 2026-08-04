#include "Npc/SessionMusicianLocalFunction.h"

#include "Library/Bgm/BgmLineFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Scene/SceneObjUtil.h"

#include "Npc/SessionMusicianManager.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"

namespace SessionMusicianLocalFunction {

SessionMusicianType getMusicianType(const al::LiveActor* actor) {
    if (al::tryGetSubActor(actor, "ドラム"))
        return SessionMusicianType::Drum;
    if (al::tryGetSubActor(actor, "ベース"))
        return SessionMusicianType::Bass;
    if (al::tryGetSubActor(actor, "ギター"))
        return SessionMusicianType::Guitar;
    if (al::tryGetSubActor(actor, "トランペット"))
        return SessionMusicianType::Trumpet;

    return SessionMusicianType::Vocal;
}

bool isMusicianType(const al::LiveActor* actor, SessionMusicianType type) {
    return (u32)getMusicianType(actor) == (u32)type;
}

bool isSubscribed(const al::LiveActor* actor, SessionMusicianType type) {
    return getSessionMusicianManager(actor)->isSubscribed(type);
}

bool isAlreadySessionMember(const SessionMusicianNpc* musician) {
    if (al::isAlive(musician))
        return false;

    GameDataHolderAccessor accessor(musician);
    s32 index = musician->getLinkedShineIndex();

    return GameDataFunction::isGotShine(accessor, index);
}

void entryMusicianToManager(SessionMusicianNpc* musician) {
    getSessionMusicianManager(musician)->entryMusician(musician);
}

bool isSessionFullMember(const al::LiveActor* actor) {
    GameDataHolderAccessor accessor(actor);
    return (SessionEventProgress::Wait4thMusician <
            GameDataFunction::getSessionEventProgress(accessor));
}

s32 getMemberMusicianNum(const al::LiveActor* actor) {
    if (isSubscribed(actor, SessionMusicianType::Vocal))
        return SessionMusicianType::size();

    s32 count = 0;

    for (s32 i = 0; i < SessionMusicianType::size(); ++i)
        if (isSubscribed(actor, i))
            count++;

    return count;
}

void startPlayingTheBa(const al::LiveActor* actor) {
    al::startBgmSituation(actor, "PlayTheBa", false, true);
}

void startPlayingTheDs(const al::LiveActor* actor) {
    al::startBgmSituation(actor, "PlayTheDs", false, true);
}

void startPlayingTheGt(const al::LiveActor* actor) {
    al::startBgmSituation(actor, "PlayTheGt", false, true);
}

void startPlayingTheTp(const al::LiveActor* actor) {
    al::startBgmSituation(actor, "PlayTheTp", false, true);
}

void endPlayingTheBa(const al::LiveActor* actor) {
    al::endBgmSituation(actor, "PlayTheBa", false);
}

void endPlayingTheDs(const al::LiveActor* actor) {
    al::endBgmSituation(actor, "PlayTheDs", false);
}

void endPlayingTheGt(const al::LiveActor* actor) {
    al::endBgmSituation(actor, "PlayTheGt", false);
}

void endPlayingTheTp(const al::LiveActor* actor) {
    al::endBgmSituation(actor, "PlayTheTp", false);
}

}  // namespace SessionMusicianLocalFunction
