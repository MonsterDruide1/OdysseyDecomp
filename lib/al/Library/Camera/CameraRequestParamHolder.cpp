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
    return mFlyerCamera && mIsFlyer;
}

void CameraRequestParamHolder::onPlayerTypeFlyer(const IUseCamera* camera, const char* name) {
    mIsFlyer = true;
    mFlyerCamera = camera;
}

bool CameraRequestParamHolder::isPlayerTypeHighSpeedMove() const {
    return mHighSpeedMoveCamera && mIsHighSpeedMove;
}

void CameraRequestParamHolder::onPlayerTypeHighSpeedMove(const IUseCamera* camera,
                                                         const char* name) {
    mIsHighSpeedMove = true;
    mHighSpeedMoveCamera = camera;
}

bool CameraRequestParamHolder::isPlayerTypeHighJump() const {
    return mHighJumpCamera && mIsHighJump;
}

void CameraRequestParamHolder::onPlayerTypeHighJump(const IUseCamera* camera, const char* name) {
    mIsHighJump = true;
    mHighJumpCamera = camera;
}

bool CameraRequestParamHolder::isPlayerTypeNotTouchGround() const {
    return mNotTouchGroundCamera && mIsNotTouchGround;
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
