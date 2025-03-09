#pragma once

namespace al {
class IUseCamera;

class CameraRequestParamHolder {
public:
    CameraRequestParamHolder();

    bool isPlayerTypeFlyer();
    bool isPlayerTypeHighSpeedMove();
    bool isPlayerTypeHighJump();
    bool isPlayerTypeNotTouchGround();

    void resetPlayerType();

    void onPlayerTypeFlyer(const IUseCamera* iUseCamera, const char* name);
    void onPlayerTypeHighSpeedMove(const IUseCamera* iUseCamera, const char* name);
    void onPlayerTypeHighJump(const IUseCamera* iUseCamera, const char* name);
    void onPlayerTypeNotTouchGround(const IUseCamera* iUseCamera, const char* name);
    void onRideObj(const IUseCamera* iUseCamera, const char* name);
    void offRideObj(const IUseCamera* iUseCamera, const char* name);

private:
    void* _0;
    bool mIsTypeFlyer;
    bool mHasBeenTypeFlyer;
    const char* mNameFlyer;
    bool mIsTypeHighSpeedMove;
    bool mHasBeenTypeHighSpeedMove;
    const char* mNameHighSpeedMove;
    bool mIsTypeHighJump;
    bool mHasBeenTypeHighJump;
    const char* mNameHighJump;
    bool mIsTypeNotTouchGround;
    bool mHasBeenTypeNotTouchGround;
    const char* mNameNotTouchGround;
    bool mIsRideObj;
    bool mHasBeenRideObj;
    const char* mNameRideObj;
    void* _58;
    void* _60;
};

static_assert(sizeof(CameraRequestParamHolder) == 0x68);

}  // namespace al
