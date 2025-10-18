#include "Library/Rail/RailRider.h"

#include "Library/Math/MathUtil.h"
#include "Library/Rail/Rail.h"

namespace al {
RailRider::RailRider(const Rail* rail) : mRail(rail) {
    syncPosDir();
}

void RailRider::moveToRailStart() {
    mCoord = 0.0f;
    syncPosDir();
}

void RailRider::move() {
    mCoord += mIsMoveForwards ? mRate : -mRate;
    syncPosDir();
}

void RailRider::syncPosDir() {
    mCoord = mRail->normalizeLength(mCoord);
    mRail->calcPosDir(&mPosition, &mDirection, mCoord);
}

void RailRider::setCoord(f32 coord) {
    mCoord = coord;
    syncPosDir();
}

void RailRider::moveToRailPoint(s32 point) {
    mCoord = mRail->getPartLength(point);
    syncPosDir();
}

void RailRider::moveToRailEnd() {
    mCoord = mRail->getTotalLength();
    syncPosDir();
}

void RailRider::moveToBegin() {
    if (mIsMoveForwards)
        moveToRailStart();
    else
        moveToRailEnd();
}

void RailRider::moveToGoal() {
    if (mIsMoveForwards)
        moveToRailEnd();
    else
        moveToRailStart();
}

void RailRider::moveToNearestRail(const sead::Vector3f& position) {
    mCoord = mRail->calcNearestRailPosCoord(position, 20.0f);
    syncPosDir();
}

void RailRider::reverse() {
    mIsMoveForwards = !mIsMoveForwards;
}

void RailRider::setMoveGoingStart() {
    mIsMoveForwards = false;
}

void RailRider::setMoveGoingEnd() {
    mIsMoveForwards = true;
}

void RailRider::setSpeed(f32 speed) {
    mRate = speed;
}

void RailRider::addSpeed(f32 speed) {
    mRate += speed;
}

void RailRider::scaleSpeed(f32 scale) {
    mRate *= scale;
}

bool RailRider::isReachedGoal() const {
    if (mIsMoveForwards)
        return isReachedRailEnd();
    else
        return isReachedRailStart();
}

bool RailRider::isReachedRailEnd() const {
    if (mRail->isClosed())
        return false;
    return isNearZero(mCoord - mRail->getTotalLength());
}

bool RailRider::isReachedRailStart() const {
    if (mRail->isClosed())
        return false;
    return isNearZero(mCoord);
}

bool RailRider::isReachedEdge() const {
    return isReachedRailStart() || isReachedRailEnd();
}

}  // namespace al
