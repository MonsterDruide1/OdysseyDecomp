#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class Rail;

class RailRider {
public:
    RailRider(const Rail* rail);
    void moveToRailStart();
    void move();
    void syncPosDir();
    void setCoord(f32 coord);
    void moveToRailPoint(s32 point);
    void moveToRailEnd();
    void moveToBegin();
    void moveToGoal();
    void moveToNearestRail(const sead::Vector3f& position);
    void reverse();
    void setMoveGoingStart();
    void setMoveGoingEnd();
    void setSpeed(f32 speed);
    void addSpeed(f32 speed);
    void scaleSpeed(f32 speed);
    bool isReachedGoal();
    bool isReachedRailEnd();
    bool isReachedRailStart();
    bool isReachedEdge();

private:
    const Rail* mRail;
    sead::Vector3f mPosition = sead::Vector3f::zero;
    sead::Vector3f mDirection = sead::Vector3f::zero;
    f32 mCoord = 0.0f;
    f32 mRate = 0.0f;
    bool mIsMoveForwards = true;
};
}  // namespace al
