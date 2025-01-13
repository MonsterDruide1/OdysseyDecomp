#pragma once
#include "Library/LiveActor/LiveActor.h"
#include "container/seadPtrArray.h"

class ElectricWire;

namespace al {
class CameraTicket;
class CameraTicketId;
class CameraPoser;
class PlacementInfo;
}

class ElectricWireRailKeeper : public al::LiveActor {
public:
    ElectricWireRailKeeper(const char *name);
    ElectricWireRailKeeper(const char *name, al::LiveActor *wire);

    void appear() override;
    void appearBySwitch();
    void endCameraIfActive();
    void exeStandby();
    void exeWait();
    al::CameraPoser* findRailPointCameraTicket(int priority) const;
    const al::CameraTicketId* findRailPointStartCameraHackEndTicket(int priority) const;
    al::PlacementInfo &getRailPointInfo(int index) const;
    void init(const al::ActorInitInfo& info) override;
    bool isNerveStandby() const;
    bool isRailPointEnableTargetEndCollision(int index) const;
    bool isRailPointFaceToCameraDir(int index) const;
    bool isRailPointIgnore(int index) const;
    bool isRailPointIsDisplayPointModelForce(int index) const;
    bool isRailPointIsExpandRailSelectableAngle(int index) const;
    bool isRailPointIsNeedCamera(int index) const;
    bool isRailPointIsNeedStartCameraHackEnd(int index) const;
    bool isRailPointPlacementPole(int index) const;
    bool isRailPointSpringFix(int index) const;
    void kill() override;
    void killBySwitch();
    bool tryGetPlayerBottomPosOnVerticalMove(sead::Vector3f *playerBottomPos) const;
    bool tryGetPlayerHeadPosOnVerticalMove(sead::Vector3f *playerHeadPos) const;
    bool tryGetRailPointDestinationTrans(sead::Vector3f * out, int index) const;
    bool tryGetRailPointFastenerMoveLimitAreaFlag(int * out, int index) const;
    bool tryGetRailPointOutDir(sead::Vector3f * out, int index) const;

    enum class PosType : u32 {
        NONE, HEAD, BOTTOM
    };
private:
    ElectricWire *mElectricWire = nullptr;
    float mUnused = 75;
    bool mIsShowLine = true;
    bool mIsThrowaway = false;
    PosType mPosType = PosType::NONE;
    sead::Vector3f mPlayerPosOnVerticalMove{};
    sead::PtrArray<al::CameraTicket> mCameraTickets{};
};

static_assert(sizeof(ElectricWireRailKeeper) == 0x138);
