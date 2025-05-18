#include "Library/Camera/CameraRequestParamHolder.h"

namespace al {

CameraRequestParamHolder::CameraRequestParamHolder() = default;

void CameraRequestParamHolder::resetPlayerType() {
    mIsFlyerCurrType = mIsFlyerPrevType;
    mFlyerCamera = nullptr;
    mIsHighSpeedMoveCurrType = mIsHighSpeedMovePrevType;
    mHighSpeedMoveCamera = nullptr;
    mIsHighJumpCurrType = mIsHighJumpPrevType;
    mHighJumpCamera = nullptr;
    mIsNotTouchGroundCurrType = mIsNotTouchGroundPrevType;
    mNotTouchGroundCamera = nullptr;
}

bool CameraRequestParamHolder::isPlayerTypeFlyer() const {
    return mFlyerCamera && mIsFlyerCurrType;
}

void CameraRequestParamHolder::onPlayerTypeFlyer(const IUseCamera* camera, const char* name) {
    mIsFlyerCurrType = true;
    mFlyerCamera = camera;
}

bool CameraRequestParamHolder::isPlayerTypeHighSpeedMove() const {
    return mHighSpeedMoveCamera && mIsHighSpeedMoveCurrType;
}

void CameraRequestParamHolder::onPlayerTypeHighSpeedMove(const IUseCamera* camera,
                                                         const char* name) {
    mIsHighSpeedMoveCurrType = true;
    mHighSpeedMoveCamera = camera;
}

bool CameraRequestParamHolder::isPlayerTypeHighJump() const {
    return mHighJumpCamera && mIsHighJumpCurrType;
}

void CameraRequestParamHolder::onPlayerTypeHighJump(const IUseCamera* camera, const char* name) {
    mIsHighJumpCurrType = true;
    mHighJumpCamera = camera;
}

bool CameraRequestParamHolder::isPlayerTypeNotTouchGround() const {
    return mNotTouchGroundCamera && mIsNotTouchGroundCurrType;
}

void CameraRequestParamHolder::onPlayerTypeNotTouchGround(const IUseCamera* camera,
                                                          const char* name) {
    mIsNotTouchGroundCurrType = true;
    mNotTouchGroundCamera = camera;
}

void CameraRequestParamHolder::onRideObj(const IUseCamera* camera, const char* name) {
    mIsRideObjCurrType = true;
    mRideObjCamera = camera;
}

void CameraRequestParamHolder::offRideObj(const IUseCamera* camera, const char* name) {
    mRideObjCamera = nullptr;
    mIsRideObjCurrType = mIsRideObjPrevType;
}

}  // namespace al
