#pragma once

#include "Npc/SessionMusicianType.h"

namespace al {
class IUseSceneObjHolder;
class LiveActor;
}  // namespace al

class SessionMusicianNpc;

namespace SessionMusicianLocalFunction {
SessionMusicianType getMusicianType(const al::LiveActor* actor);
bool isMusicianType(const al::LiveActor* actor, SessionMusicianType type);
bool isSubscribed(const al::LiveActor* actor, SessionMusicianType type);
bool isAlreadySessionMember(const SessionMusicianNpc* musician);
void entryMusicianToManager(SessionMusicianNpc* musician);
s32 getMemberMusicianNum(const al::LiveActor* actor);
bool isSessionFullMember(const al::LiveActor* actor);
void startPlayingTheBa(const al::LiveActor* actor);
void startPlayingTheDs(const al::LiveActor* actor);
void startPlayingTheGt(const al::LiveActor* actor);
void startPlayingTheTp(const al::LiveActor* actor);
void endPlayingTheBa(const al::LiveActor* actor);
void endPlayingTheDs(const al::LiveActor* actor);
void endPlayingTheGt(const al::LiveActor* actor);
void endPlayingTheTp(const al::LiveActor* actor);
}  // namespace SessionMusicianLocalFunction
