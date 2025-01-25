#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class MtxConnector;

class VisibleSwitchMapParts : public LiveActor {
public:
    VisibleSwitchMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    /**
     * @warning This function produce Undefined Behavior when mConnectMapDirType is equal to
     * ConnectMapDirType::Invalid, ConnectMapDirType::NegSide, ConnectMapDirType::NegUp or
     * ConnectMapDirType::NegFront
     */
    void initAfterPlacement() override;
    void control() override;
    bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) override;

    void startDisappear();
    void startAppear();
    void startSuddenDisappear();

    void exeShow();
    void exeDisappear();
    void exeDisappearDither();
    void exeHide();
    void exeAppear();
    void exeAppearDither();

    enum class ConnectMapDirType : s32 {
        Invalid = -1,
        Side = 0,
        NegSide = 1,
        Up = 2,
        NegUp = 3,
        Front = 4,
        NegFront = 5
    };

private:
    ConnectMapDirType mConnectMapDirType = ConnectMapDirType::Invalid;
    MtxConnector* mMtxConnector = nullptr;
    f32 mConnectMapCheckLength = 150.0f;
    bool mIsUsingAlphaMask = true;
    s32 mDitherFrame = 30;
};

static_assert(sizeof(VisibleSwitchMapParts) == 0x128);
}  // namespace al
