#include "Library/Camera/CameraRequestParamHolder.h"

namespace al {

CameraRequestParamHolder::CameraRequestParamHolder() = default;

void CameraRequestParamHolder::resetPlayerType() {
    mIsFlyer = _9;
    mFlyerCamera = nullptr;
    mIsHighSpeedMove = _19;
    mHighSpeedMoveCamera = nullptr;
    mIsHighJump = _29;
    mHighJumpCamera = nullptr;
    mIsNotTouchGround = _39;
    mNotTouchGroundCamera = nullptr;
}

bool CameraRequestParamHolder::isPlayerTypeFlyer() const {
    if (mFlyerCamera)
        return mIsFlyer;
    return false;
}

void CameraRequestParamHolder::onPlayerTypeFlyer(const IUseCamera* camera, const char* name) {
    mIsFlyer = true;
    mFlyerCamera = camera;
}

bool CameraRequestParamHolder::isPlayerTypeHighSpeedMove() const {
    if (mHighSpeedMoveCamera)
        return mIsHighSpeedMove;
    return false;
}

void CameraRequestParamHolder::onPlayerTypeHighSpeedMove(const IUseCamera* camera,
                                                         const char* name) {
    mIsHighSpeedMove = true;
    mHighSpeedMoveCamera = camera;
}

bool CameraRequestParamHolder::isPlayerTypeHighJump() const {
    if (mHighJumpCamera)
        return mIsHighJump;
    return false;
}

void CameraRequestParamHolder::onPlayerTypeHighJump(const IUseCamera* camera, const char* name) {
    mIsHighJump = true;
    mHighJumpCamera = camera;
}

bool CameraRequestParamHolder::isPlayerTypeNotTouchGround() const {
    if (mNotTouchGroundCamera)
        return mIsNotTouchGround;
    return false;
}

void CameraRequestParamHolder::onPlayerTypeNotTouchGround(const IUseCamera* camera,
                                                          const char* name) {
    mIsNotTouchGround = true;
    mNotTouchGroundCamera = camera;
}

void CameraRequestParamHolder::onRideObj(const IUseCamera* camera, const char* name) {
    mIsRideObj = true;
    mRideObjCamera = camera;
}

void CameraRequestParamHolder::offRideObj(const IUseCamera* camera, const char* name) {
    mRideObjCamera = nullptr;
    mIsRideObj = _49;
}

}  // namespace al
