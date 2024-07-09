#pragma once

#include <math/seadMatrix.h>
#include <prim/seadSafeString.h>

namespace al {
class LiveActor;
class Resource;
class HitSensor;

LiveActor* createLinksActorFromFactory(const ActorInitInfo& info, const char* linkName,
                                       s32 linkNum);
bool trySyncStageSwitchAppear(LiveActor* actor);
void initActorPoseTRSV(LiveActor* actor);
void initActorSRT(LiveActor* actor, const ActorInitInfo& info);
bool trySyncStageSwitchAppearAndKill(LiveActor* actor);
void initActorCollisionWithResource(LiveActor* actor, const Resource* res,
                                    const sead::SafeString& collisionFileName, HitSensor* hitSensor,
                                    const sead::Matrix34f* joinMtx, const char* suffix);
void initExecutorCollisionMapObjDecorationMovement(LiveActor* actor, const ActorInitInfo& info);
void initActorModelKeeperByHost(LiveActor* actor, const LiveActor* parent);
void initExecutorDraw(LiveActor* actor, const ActorInitInfo& info, const char* executorDrawName);
void initChildActorWithArchiveNameWithPlacementInfo(LiveActor* actor, const ActorInitInfo& info,
                                                    const sead::SafeString& archiveName,
                                                    const char* suffix);
bool tryListenStageSwitchKill(LiveActor* actor);
void initActorWithArchiveName(LiveActor* actor, const ActorInitInfo& info,
                              const sead::SafeString& archiveName, const char* suffix);
void initActorEffectKeeper(LiveActor* actor, const ActorInitInfo& info, const char* name);
void initActorPoseTQSV(LiveActor* actor);
void initExecutorUpdate(LiveActor* actor, const ActorInitInfo& info, const char* name);
}  // namespace al
