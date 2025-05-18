#pragma once

#include <basis/seadTypes.h>

namespace al {
class IUseCamera;

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

    s32 getStickSensitivityLevel() const { return mStickSensitivityLevel; }

    void setStickSensitivityLevel(s32 sensitivityLevel) {
        mStickSensitivityLevel = sensitivityLevel;
    }

    s32 getGyroSensitivityLevel() const { return mGyroSensitivityLevel; }

    void setGyroSensitivityLevel(s32 sensitivityLevel) { mGyroSensitivityLevel = sensitivityLevel; }

private:
    s32 mStickSensitivityLevel = 0;
    s32 mGyroSensitivityLevel = 0;
    bool mIsFlyer = false;
    bool _9 = false;
    const IUseCamera* mFlyerCamera = nullptr;
    bool mIsHighSpeedMove = false;
    bool _19 = false;
    const IUseCamera* mHighSpeedMoveCamera = nullptr;
    bool mIsHighJump = false;
    bool _29 = false;
    const IUseCamera* mHighJumpCamera = nullptr;
    bool mIsNotTouchGround = false;
    bool _39 = false;
    const IUseCamera* mNotTouchGroundCamera = nullptr;
    bool mIsRideObj = false;
    bool _49 = false;
    const IUseCamera* mRideObjCamera = nullptr;
    void* _58 = nullptr;
    void* _60 = nullptr;
};

}  // namespace al
