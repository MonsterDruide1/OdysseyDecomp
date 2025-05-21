#include "Library/Camera/CameraRequestParamHolder.h"

namespace al {

CameraRequestParamHolder::CameraRequestParamHolder() = default;

void CameraRequestParamHolder::resetPlayerType() {
    mIsCurrFlyer = mIsPrevFlyer;
    mFlyerCamera = nullptr;
    mIsCurrHighSpeedMove = mIsPrevHighSpeedMove;
    mHighSpeedMoveCamera = nullptr;
    mIsCurrHighJump = mIsPrevHighJump;
    mHighJumpCamera = nullptr;
    mIsCurrNotTouchGround = mIsPrevNotTouchGround;
    mNotTouchGroundCamera = nullptr;
}

bool CameraRequestParamHolder::isPlayerTypeFlyer() const {
    return mFlyerCamera && mIsCurrFlyer;
}

void CameraRequestParamHolder::onPlayerTypeFlyer(const IUseCamera* camera, const char* name) {
    mIsCurrFlyer = true;
    mFlyerCamera = camera;
}

bool CameraRequestParamHolder::isPlayerTypeHighSpeedMove() const {
    return mHighSpeedMoveCamera && mIsCurrHighSpeedMove;
}

void CameraRequestParamHolder::onPlayerTypeHighSpeedMove(const IUseCamera* camera,
                                                         const char* name) {
    mIsCurrHighSpeedMove = true;
    mHighSpeedMoveCamera = camera;
}

bool CameraRequestParamHolder::isPlayerTypeHighJump() const {
    return mHighJumpCamera && mIsCurrHighJump;
}

void CameraRequestParamHolder::onPlayerTypeHighJump(const IUseCamera* camera, const char* name) {
    mIsCurrHighJump = true;
    mHighJumpCamera = camera;
}

bool CameraRequestParamHolder::isPlayerTypeNotTouchGround() const {
    return mNotTouchGroundCamera && mIsCurrNotTouchGround;
}

void CameraRequestParamHolder::onPlayerTypeNotTouchGround(const IUseCamera* camera,
                                                          const char* name) {
    mIsCurrNotTouchGround = true;
    mNotTouchGroundCamera = camera;
}

void CameraRequestParamHolder::onRideObj(const IUseCamera* camera, const char* name) {
    mIsCurrRideObj = true;
    mRideObjCamera = camera;
}

void CameraRequestParamHolder::offRideObj(const IUseCamera* camera, const char* name) {
    mRideObjCamera = nullptr;
    mIsCurrRideObj = mIsPrevRideObj;
}

}  // namespace al
