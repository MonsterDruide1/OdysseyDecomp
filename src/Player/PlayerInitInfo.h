#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class GamePadSystem;
}

class PlayerInitInfo {
public:
    al::GamePadSystem* getGamePadSystem() const { return mGamePadSystem; }

    sead::Matrix34f* getViewMtx() const { return mViewMtx; }

    u32 getPortNo() const { return mPortNo; }

    const char* getModelName() const { return mModelName; }

    const char* getCapTypeName() const { return mCapTypeName; }

    const sead::Vector3f& getTrans() const { return mTrans; }

    const sead::Quatf& getQuat() const { return mQuat; }

    bool isNeedCreateNoseNeedle() const { return mIsNeedCreateNoseNeedle; }

    bool isClosetScenePlayer() const { return mIsClosetScenePlayer; }

private:
    al::GamePadSystem* mGamePadSystem = nullptr;
    sead::Matrix34f* mViewMtx = nullptr;
    u32 mPortNo = 0;
    const char* mModelName = nullptr;
    const char* mCapTypeName = nullptr;
    sead::Vector3f mTrans = sead::Vector3f::zero;
    sead::Quatf mQuat = sead::Quatf::unit;
    bool mIsNeedCreateNoseNeedle = false;
    bool mIsClosetScenePlayer = false;
};

static_assert(sizeof(PlayerInitInfo) == 0x48);
