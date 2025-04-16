#pragma once

#include <container/seadPtrArray.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

class ElectricWire;

namespace al {
class CameraTicket;
class PlacementInfo;
}  // namespace al

class ElectricWireRailKeeper : public al::LiveActor {
public:
    enum class PosType : u32 { NONE, HEAD, BOTTOM };

    struct TicketHolder {
        al::CameraTicket* ticket;
        al::CameraTicket* ticketHackEnd;
        s32 pointIdx;
    };

    ElectricWireRailKeeper(const char* name);
    ElectricWireRailKeeper(const char* name, al::LiveActor* wire);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void kill() override;

    bool isRailPointFaceToCameraDir(s32 index) const;
    bool isRailPointPlacementPole(s32 index) const;
    bool isRailPointEnableTargetEndCollision(s32 index) const;
    bool isRailPointIgnore(s32 index) const;
    bool isRailPointSpringFix(s32 index) const;
    bool isRailPointIsNeedCamera(s32 index) const;
    bool isRailPointIsNeedStartCameraHackEnd(s32 index) const;
    bool isRailPointIsExpandRailSelectableAngle(s32 index) const;
    bool isRailPointIsDisplayPointModelForce(s32 index) const;
    bool tryGetRailPointOutDir(sead::Vector3f* out, s32 index) const;
    bool tryGetRailPointDestinationTrans(sead::Vector3f* out, s32 index) const;
    bool tryGetRailPointFastenerMoveLimitAreaFlag(s32* out, s32 index) const;
    al::CameraTicket* findRailPointCameraTicket(s32 pointIdx) const;
    const al::CameraTicket* findRailPointStartCameraHackEndTicket(s32 pointIdx) const;
    bool tryGetPlayerHeadPosOnVerticalMove(sead::Vector3f* playerHeadPos) const;
    bool tryGetPlayerBottomPosOnVerticalMove(sead::Vector3f* playerBottomPos) const;
    void endCameraIfActive();
    al::PlacementInfo* getRailPointInfo(s32 index) const;
    void appearBySwitch();
    void killBySwitch();
    void exeStandby();
    void exeWait();
    bool isNerveStandby() const;

private:
    ElectricWire* mElectricWire = nullptr;
    f32 mUnused = 75;
    bool mIsShowLine = true;
    bool mIsThrowaway = false;
    PosType mPosType = PosType::NONE;
    sead::Vector3f mPlayerPosOnVerticalMove = sead::Vector3f::zero;
    sead::PtrArray<TicketHolder> mCameraTickets{};
};

static_assert(sizeof(ElectricWireRailKeeper) == 0x138);
