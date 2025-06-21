#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;
class AreaInitInfo;
class AreaObj;
class AreaObjGroup;
class IUseAreaObj;
class LiveActor;
class PlacementInfo;
class PlayerHolder;
class SwitchKeepOnAreaGroup;
class SwitchOnAreaGroup;
class ValidatorBase;

AreaObj* tryFindAreaObjPlayerOne(const PlayerHolder* holder, const char* name);
AreaObj* tryFindAreaObjPlayerAll(const PlayerHolder* holder, const char* name);
AreaObj* tryGetAreaObjPlayerAll(const LiveActor* actor, const AreaObjGroup* areaGroup);
bool isInAreaObjPlayerAll(const LiveActor* actor, const AreaObj* areaObj);
bool isInAreaObjPlayerAll(const LiveActor* actor, const AreaObjGroup* areaGroup);
bool isInAreaObjPlayerAnyOne(const LiveActor* actor, const AreaObj* areaObj);
bool isInAreaObjPlayerAnyOne(const LiveActor* actor, const AreaObjGroup* areaGroup);
AreaObj* createAreaObj(const ActorInitInfo& actorInitInfo, const char* name);
void initAreaInitInfo(AreaInitInfo* areaInitInfo, const ActorInitInfo& actorInitInfo);
AreaObj* createLinkArea(const ActorInitInfo& initInfo, const char* name, const char* areaName);
void initAreaInitInfo(AreaInitInfo* areaInitInfo, const PlacementInfo& placementInfo,
                      const ActorInitInfo& actorInitInfo);
AreaObj* tryCreateLinkArea(const ActorInitInfo& initInfo, const char* name, const char* areaName);
AreaObjGroup* createLinkAreaGroup(const ActorInitInfo& initInfo, const char* name,
                                  const char* groupName, const char* areaName);
AreaObjGroup* createLinkAreaGroup(LiveActor* actor, const ActorInitInfo& initInfo, const char* name,
                                  const char* groupName, const char* areaName);
AreaObj* tryFindAreaObj(const LiveActor* actor, const char* name);
bool isInAreaObj(const LiveActor* actor, const char* name);
bool isInAreaObjPlayerOne(const PlayerHolder* holder, const char* name);
bool isInAreaObjPlayerAll(const PlayerHolder* holder, const char* name);
bool isInAreaObjPlayerOneIgnoreAreaTarget(const PlayerHolder* holder, const char* name);
bool isInDeathArea(const LiveActor* actor);
bool isInWaterArea(const LiveActor* actor);
bool isInPlayerControlOffArea(const LiveActor* actor);
f32 calcWaterSinkDepth(const LiveActor* actor);
void registerAreaHostMtx(const IUseAreaObj* areaObj, const sead::Matrix34f* mtx,
                         const ActorInitInfo& initInfo);
void registerAreaHostMtx(const IUseAreaObj* areaObj, const sead::Matrix34f* mtx,
                         const ActorInitInfo& initInfo, const ValidatorBase& validator);
void registerAreaHostMtx(const LiveActor* actor, const ActorInitInfo& initInfo);
void registerAreaHostMtx(const LiveActor* actor, const ActorInitInfo& initInfo,
                         const ValidatorBase& validator);
void registerAreaSyncHostMtx(const IUseAreaObj* areaObj, const sead::Matrix34f* mtx,
                             const ActorInitInfo& initInfo);
void registerAreaSyncHostMtx(const IUseAreaObj* areaObj, const sead::Matrix34f* mtx,
                             const ActorInitInfo& initInfo, const ValidatorBase& validator);
void registerAreaSyncHostMtx(const LiveActor* actor, const ActorInitInfo& initInfo);
void registerAreaSyncHostMtx(const LiveActor* actor, const ActorInitInfo& initInfo,
                             const ValidatorBase& validator);
bool tryReviseVelocityInsideAreaObj(sead::Vector3f* nearestEdgePos, LiveActor* actor,
                                    AreaObjGroup* areaGroup, const AreaObj* areaObj);
SwitchKeepOnAreaGroup* tryCreateSwitchKeepOnAreaGroup(LiveActor* actor,
                                                      const ActorInitInfo& initInfo);
SwitchOnAreaGroup* tryCreateSwitchOnAreaGroup(LiveActor* actor, const ActorInitInfo& initInfo);

}  // namespace al
