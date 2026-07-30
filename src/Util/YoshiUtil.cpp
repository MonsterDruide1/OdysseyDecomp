#include "Util/YoshiUtil.h"

#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/PartsInterpolator.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nature/WaterSurfaceFinder.h"

#include "Player/PlayerConst.h"
#include "Player/YoshiTongue.h"

namespace rs {
bool isSensorTypeYoshiEnableSendPush(const al::HitSensor* sensor) {
    return al::isSensorNpc(sensor);
}

bool isSensorTypeYoshiMsgReceivable(const al::HitSensor* sensor) {
    return al::isSensorNpc(sensor);
}

bool isInPuddleHeight(const al::WaterSurfaceFinder* surfaceFinder, const PlayerConst* playerConst) {
    return surfaceFinder->isFoundSurface() &&
           surfaceFinder->getDistance() < playerConst->getSwimCenterOffset();
}

bool tryCalcTonguePullPose(sead::Quatf* pose, const al::LiveActor* actor,
                           const YoshiTongue* yoshiTongue) {
    sead::Vector3f pullDistance = {0.0f, 0.0f, 0.0f};
    if (yoshiTongue->tryCalcTonguePullDistance(&pullDistance)) {
        sead::Vector3f pullDir = {0.0f, 0.0f, 0.0f};
        if (al::tryNormalizeOrZero(&pullDir, pullDistance)) {
            sead::Vector3f upDir = {0.0f, 0.0f, 0.0f};
            al::calcUpDir(&upDir, actor);
            if (al::isParallelDirection(upDir, pullDir, 0.01f)) {
                al::calcFrontDir(&upDir, actor);
                upDir.negate();
            }

            al::makeQuatFrontUp(pose, pullDir, upDir);
            return true;
        }
    }

    return false;
}

bool findClingGroundPos(sead::Vector3f* groundPos, const al::LiveActor* actor,
                        const sead::Vector3f& pos, f32 height) {
    const sead::Vector3f& gravity = al::getGravity(actor);
    al::TriangleFilterGroundOnly filter(gravity);

    return alCollisionUtil::getHitPosOnArrow(actor, groundPos, pos, gravity * height, nullptr,
                                             &filter);
}
}  // namespace rs
