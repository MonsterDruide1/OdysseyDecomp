#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Npc/SessionMusicianType.h"

namespace al {
class IUseSceneObjHolder;
class LiveActor;
class PlacementInfo;
}  // namespace al

class SessionMayorNpc;
class SessionMusicianManager;
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
bool tryCreateSessionMusicianManager(const al::IUseSceneObjHolder* user);
SessionMusicianManager* getSessionMusicianManager(const al::IUseSceneObjHolder* user);
bool isExistSessionMusicianManager(const al::IUseSceneObjHolder* user);
bool tryStartWarpToSessionMayor(const al::IUseSceneObjHolder* user, al::PlacementInfo* info);
void entrySessionMayorToManager(SessionMayorNpc* mayor);
bool isJoinedSessionMusician(const al::IUseSceneObjHolder* user);
al::LiveActor* tryGetJoinedSessionMusicanActor(const al::IUseSceneObjHolder* user);
bool tryAddJoinedSessionMusicianDemoActor(const al::IUseSceneObjHolder* user);
bool tryGetSessionMoonGetDemoPlayerPos(sead::Vector3f* pos, const al::IUseSceneObjHolder* user);
bool tryGetSessionMoonGetDemoPlayerPose(sead::Quatf* quat, const al::IUseSceneObjHolder* user);
bool trySetJoinedSessionMusicianTransformForMoonGetDemo(const al::IUseSceneObjHolder* user);
void addDemoAllMusicians(const al::IUseSceneObjHolder* user);
}  // namespace SessionMusicianLocalFunction
