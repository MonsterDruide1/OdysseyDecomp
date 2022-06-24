#pragma once

#include "al/actor/ActorInitInfo.h"
#include "al/actor/Placement.h"
#include "math/seadVector.h"

namespace al {
int getCountPlacementInfo(const al::PlacementInfo&);
void getPlacementInfoByKey(al::PlacementInfo*, const al::PlacementInfo&, const char*);
void getPlacementInfoByIndex(al::PlacementInfo*, const al::PlacementInfo&, int);
bool tryGetPlacementInfoByIndex(al::PlacementInfo*, const al::PlacementInfo&, int);
bool tryGetPlacementInfoByKey(al::PlacementInfo*, const al::PlacementInfo&, const char*);
void getPlacementInfoAndKeyNameByIndex(al::PlacementInfo*, const char**, const al::PlacementInfo&,
                                       int);
bool tryGetPlacementInfoAndKeyNameByIndex(al::PlacementInfo*, const char**,
                                          const al::PlacementInfo&, int);
al::PlacementId* createPlacementId(const al::ActorInitInfo&);
al::PlacementId* createPlacementId(const al::PlacementInfo&);
bool tryGetPlacementId(al::PlacementId*, const al::ActorInitInfo&);
bool tryGetPlacementId(al::PlacementId*, const al::PlacementInfo&);
void getPlacementId(al::PlacementId*, const al::ActorInitInfo&);
bool isEqualPlacementId(const al::PlacementId&, const al::PlacementId&);
bool isEqualPlacementId(const al::PlacementInfo&, const al::PlacementInfo&);

bool tryGetRailPointPos(sead::Vector3f*, const al::PlacementInfo&);
bool getRailPointHandlePrev(sead::Vector3f*, const al::PlacementInfo&);
bool getRailPointHandleNext(sead::Vector3f*, const al::PlacementInfo&);
};  // namespace al
