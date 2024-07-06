#pragma once

#include <prim/seadSafeString.h>

namespace al {
class LiveActor;
class Resource;

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
}  // namespace al
