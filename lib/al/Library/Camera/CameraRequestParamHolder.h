#pragma once

#include <basis/seadTypes.h>

namespace al {
class IUseCamera;
class CameraDistanceCurve;

class CameraRequestParamHolder {
public:
    CameraRequestParamHolder();

    void resetPlayerType();

    bool isPlayerTypeFlyer() const;
    void onPlayerTypeFlyer(const IUseCamera* camera, const char* name);

    bool isPlayerTypeHighSpeedMove() const;
    void onPlayerTypeHighSpeedMove(const IUseCamera* camera, const char* name);

    bool isPlayerTypeHighJump() const;
    void onPlayerTypeHighJump(const IUseCamera* camera, const char* name);

    bool isPlayerTypeNotTouchGround() const;
    void onPlayerTypeNotTouchGround(const IUseCamera* camera, const char* name);

    void onRideObj(const IUseCamera* camera, const char* name);
    void offRideObj(const IUseCamera* camera, const char* name);

    // getters
    s32 getStickSensitivityLevel() const { return mStickSensitivityLevel; }

    void setStickSensitivityLevel(s32 level) { mStickSensitivityLevel = level; }

    s32 getGyroSensitivityLevel() const { return mGyroSensitivityLevel; }

    void setGyroSensitivityLevel(s32 level) { mGyroSensitivityLevel = level; }

    CameraDistanceCurve* getBossDistanceCurve() const { return mBossDistanceCurve; }

    CameraDistanceCurve* getEquipmentDistanceCurve() const { return mEquipmentDistanceCurve; }

    bool isOnRideObj() const { return mRideObjCamera && mIsCurrRideObj; }

private:
    s32 mStickSensitivityLevel = 0;
    s32 mGyroSensitivityLevel = 0;
    bool mIsCurrFlyer = false;
    bool mIsPrevFlyer = false;
    const IUseCamera* mFlyerCamera = nullptr;
    bool mIsCurrHighSpeedMove = false;
    bool mIsPrevHighSpeedMove = false;
    const IUseCamera* mHighSpeedMoveCamera = nullptr;
    bool mIsCurrHighJump = false;
    bool mIsPrevHighJump = false;
    const IUseCamera* mHighJumpCamera = nullptr;
    bool mIsCurrNotTouchGround = false;
    bool mIsPrevNotTouchGround = false;
    const IUseCamera* mNotTouchGroundCamera = nullptr;
    bool mIsCurrRideObj = false;
    bool mIsPrevRideObj = false;
    const IUseCamera* mRideObjCamera = nullptr;
    CameraDistanceCurve* mEquipmentDistanceCurve = nullptr;
    CameraDistanceCurve* mBossDistanceCurve = nullptr;
};

static_assert(sizeof(CameraRequestParamHolder) == 0x68);
}  // namespace al
