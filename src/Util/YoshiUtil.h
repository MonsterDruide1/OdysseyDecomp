#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class HitSensor;
class LiveActor;
class WaterSurfaceFinder;
}  // namespace al

class PlayerConst;
class YoshiTongue;

namespace rs {
bool isSensorTypeYoshiEnableSendPush(const al::HitSensor* sensor);
bool isSensorTypeYoshiMsgReceivable(const al::HitSensor* sensor);
bool isInPuddleHeight(const al::WaterSurfaceFinder* surfaceFinder, const PlayerConst* playerConst);
bool tryCalcTonguePullPose(sead::Quatf* pose, const al::LiveActor* actor,
                           const YoshiTongue* yoshiTongue);
bool findClingGroundPos(sead::Vector3f* pos, const al::LiveActor* actor, const sead::Vector3f&,
                        f32);
}  // namespace rs
