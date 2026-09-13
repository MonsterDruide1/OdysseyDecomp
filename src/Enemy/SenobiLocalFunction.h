#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

class IUsePlayerCollision;
class IUsePlayerHack;
class PlayerCollisionCheckSphereMove;
class Senobi;

namespace al {
class LiveActor;
}  // namespace al

namespace SenobiLocalFunction {

void setTransToStretchPoint(Senobi* senobi);
void shrinkBody(Senobi* senobi, f32 rate);
void calcBodyPos(sead::Vector3f* bodyPos, const al::LiveActor* actor);
bool isHitCeil(sead::Vector3f* hitPos, const al::LiveActor* actor,
               PlayerCollisionCheckSphereMove* collisionCheck, const sead::Vector3f& start,
               const sead::Vector3f& end, f32 radius);
bool isHitWall(sead::Vector3f* normal, al::LiveActor* actor,
               PlayerCollisionCheckSphereMove* collisionCheck, const sead::Vector3f& move);
void invalidateAllStretchSensors(al::LiveActor* actor);
void updateStretchSensors(al::LiveActor* actor, f32 stretchLength);
bool isHoldStretchButton(const IUsePlayerHack* playerHack);
bool trySendMsgToLandingGround(al::LiveActor* actor, IUsePlayerCollision* collision);
al::LiveActor* getLeaf(const al::LiveActor* actor, s32 index);
s32 calcLeavesNum(const al::LiveActor* actor);
bool tryTurnToPlayer(al::LiveActor* actor, f32 turnDegree);
void hideSenobi(Senobi* senobi);
void showSenobi(Senobi* senobi);
f32 getSenobiStretchLength(const Senobi* senobi);
void setSenobiStretchLength(Senobi* senobi, f32 stretchLength);

}  // namespace SenobiLocalFunction
