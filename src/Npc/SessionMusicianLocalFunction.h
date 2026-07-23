#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Npc/SessionMusicianType.h"

namespace al {
class LiveActor;
class PlacementInfo;
class IUseSceneObjHolder;
}  // namespace al
class SessionMayorNpc;
class SessionMusicianNpc;
class SessionMusicianManager;

namespace SessionMusicianLocalFunction {
bool isMusicianType(const al::LiveActor* actor, SessionMusicianType type);
bool isSubscribed(const al::LiveActor* actor, SessionMusicianType type);
bool isAlreadySessionMember(const SessionMusicianNpc* npc);
bool isSessionFullMember(const al::LiveActor* actor);
bool isJoinedSessionMusician(const al::IUseSceneObjHolder* holder);
bool isExistSessionMusicianManager(const al::IUseSceneObjHolder* holder);

SessionMusicianType getMusicianType(const al::LiveActor* actor);
s32 getMemberMusicianNum(const al::LiveActor* actor);
SessionMusicianManager* getSessionMusicianManager(const al::IUseSceneObjHolder* hoolder);

void entrySessionMayorToManager(SessionMayorNpc* mayor);
void entryMusicianToManager(SessionMusicianNpc* musician);

void tryCreateSessionMusicianManager(const al::IUseSceneObjHolder* holder);
bool tryStartWarpToSessionMayor(const al::IUseSceneObjHolder* holder, al::PlacementInfo* info);
bool tryAddJoinedSessionMusicianDemoActor(const al::IUseSceneObjHolder* holder);
SessionMusicianNpc* tryGetJoinedSessionMusicanActor(const al::IUseSceneObjHolder* holder);
bool tryGetSessionMoonGetDemoPlayerPos(sead::Vector3f* pos, const al::IUseSceneObjHolder* holder);
bool tryGetSessionMoonGetDemoPlayerPose(sead::Quatf* pose, const al::IUseSceneObjHolder* holder);
bool trySetJoinedSessionMusicianTransformForMoonGetDemo(const al::IUseSceneObjHolder* holder);

void addDemoAllMusicians(const al::IUseSceneObjHolder* holder);

void startPlayingTheBa(const al::LiveActor* actor);
void startPlayingTheDs(const al::LiveActor* actor);
void startPlayingTheGt(const al::LiveActor* actor);
void startPlayingTheTp(const al::LiveActor* actor);

void endPlayingTheBa(const al::LiveActor* actor);
void endPlayingTheDs(const al::LiveActor* actor);
void endPlayingTheGt(const al::LiveActor* actor);
void endPlayingTheTp(const al::LiveActor* actor);
}  // namespace SessionMusicianLocalFunction
