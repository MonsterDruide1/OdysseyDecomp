#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class HitSensor;
class MtxConnector;
class PlacementId;
class SensorMsg;
}  // namespace al

class CoinCollectEmpty;
class CoinCollectHintState;
class CoinRotateCalculator;
class CoinStateCountUp;
class ExternalForceKeeper;
class WaterSurfaceShadow;

class CoinCollect : public al::LiveActor {
public:
    CoinCollect(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void control() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void endClipped() override;
    void appear() override;
    void makeActorAlive() override;

    void listenKill();
    void listenAppear();
    const sead::Vector3f& getTransForHint() const;
    void appearHelpAmiiboEffect();
    void deleteHelpAmiiboEffect();
    void reappearHelpAmiiboEffect();
    bool isEnableHint() const;
    void rotate();

    void exeWait();
    void exeWaitAmiibo();
    void exeGot();
    void exeCountUp();
    void exeBlow();

    bool isHintEnabled() const { return mIsHintEnabled; }

private:
    CoinStateCountUp* mStateCountUp = nullptr;
    CoinCollectHintState* mHintState = nullptr;
    al::PlacementId* mPlacementId = nullptr;
    sead::Quatf mQuat = sead::Quatf::unit;
    al::MtxConnector* mMtxConnector = nullptr;
    ExternalForceKeeper* mExternalForceKeeper = nullptr;
    CoinRotateCalculator* mRotateCalculator = nullptr;
    f32 mShadowLength = 0.0f;
    WaterSurfaceShadow* mWaterSurfaceShadow = nullptr;
    bool mIsSurfaceUpdated = false;
    sead::Matrix34f mSurfaceMatrix = sead::Matrix34f::ident;
    CoinCollectEmpty* mCoinCollectEmpty = nullptr;
    bool mIsHintEnabled = false;
};
