#include "ElectricWireRailKeeper.h"

#include "ElectricWire.h"
#include "Library/Camera/CameraTicket.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Rail/RailUtil.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Thread/FunctorV0M.h"

namespace {
NERVE_IMPL(ElectricWireRailKeeper, Standby)
NERVE_IMPL(ElectricWireRailKeeper, Wait)

NERVES_MAKE_NOSTRUCT(ElectricWireRailKeeper, Standby, Wait)
}  // namespace

void ElectricWireRailKeeper::appear() {
    LiveActor::appear();
    this->mElectricWire->tryUpdateDisplayModel();
    al::setNerve(this, &Wait);
}

void ElectricWireRailKeeper::appearBySwitch() {
    if (al::isAlive(this))
        return;

    this->appear();
    al::invalidateClipping(this);
    al::setNerve(this, &Standby);
}

void ElectricWireRailKeeper::exeStandby() {
    if (al::isGreaterStep(this, 10)) {
        al::validateClipping(this);
        al::setNerve(this, &Wait);
    }
}

void ElectricWireRailKeeper::exeWait() {}

bool ElectricWireRailKeeper::isNerveStandby() const {
    return al::isNerve(this, &Standby);
}

al::PlacementInfo& ElectricWireRailKeeper::getRailPointInfo(int index) const {
    return al::getRailPointInfo(this, index);
}

bool ElectricWireRailKeeper::isRailPointEnableTargetEndCollision(int index) const {
    al::PlacementInfo& info = al::getRailPointInfo(this, index);
    bool ret = true;
    al::tryGetArg(&ret, info, "IsEnableTargetEndCollision");
    return ret;
}

bool ElectricWireRailKeeper::isRailPointFaceToCameraDir(int index) const {
    al::PlacementInfo& info = al::getRailPointInfo(this, index);
    bool ret;
    if (al::tryGetArg(&ret, info, "IsFaceToCamera"))
        return ret;
    return false;
}

bool ElectricWireRailKeeper::isRailPointIgnore(int index) const {
    al::PlacementInfo& info = al::getRailPointInfo(this, index);
    bool ret;
    if (al::tryGetArg(&ret, info, "IsIgnore"))
        return ret;
    return false;
}

bool ElectricWireRailKeeper::isRailPointIsDisplayPointModelForce(int index) const {
    al::PlacementInfo& info = al::getRailPointInfo(this, index);
    bool ret = false;
    al::tryGetArg(&ret, info, "IsDisplayPointModelForce");
    return ret;
}

bool ElectricWireRailKeeper::isRailPointIsExpandRailSelectableAngle(int index) const {
    al::PlacementInfo& info = al::getRailPointInfo(this, index);
    bool ret = false;
    al::tryGetArg(&ret, info, "IsExpandRailSelectableAngle");
    return ret;
}

bool ElectricWireRailKeeper::isRailPointIsNeedCamera(int index) const {
    al::PlacementInfo& info = al::getRailPointInfo(this, index);
    bool ret = false;
    al::tryGetArg(&ret, info, "IsNeedCamera");
    return ret;
}

bool ElectricWireRailKeeper::isRailPointIsNeedStartCameraHackEnd(int index) const {
    al::PlacementInfo& info = al::getRailPointInfo(this, index);
    bool ret = false;
    al::tryGetArg(&ret, info, "IsNeedStartCameraHackEnd");
    return ret;
}

bool ElectricWireRailKeeper::isRailPointPlacementPole(int index) const {
    al::PlacementInfo& info = al::getRailPointInfo(this, index);
    bool ret;
    if (al::tryGetArg(&ret, info, "IsPlacementPole"))
        return ret;
    return false;
}

bool ElectricWireRailKeeper::isRailPointSpringFix(int index) const {
    al::PlacementInfo& info = al::getRailPointInfo(this, index);
    bool ret;
    if (al::tryGetArg(&ret, info, "IsSpringFix"))
        return ret;
    return false;
}

void ElectricWireRailKeeper::kill() {
    LiveActor::kill();
    this->mElectricWire->tryUpdateDisplayModel();
}

void ElectricWireRailKeeper::killBySwitch() {
    if (al::isDead(this))
        return;
    this->kill();
}

bool ElectricWireRailKeeper::tryGetPlayerBottomPosOnVerticalMove(
    sead::Vector3f* playerBottomPos) const {
    if (this->mPosType == PosType::BOTTOM) {
        playerBottomPos->set(mPlayerPosOnVerticalMove);
        return true;
    }
    return false;
}

bool ElectricWireRailKeeper::tryGetPlayerHeadPosOnVerticalMove(
    sead::Vector3f* playerHeadPos) const {
    if (this->mPosType == PosType::HEAD) {
        playerHeadPos->set(mPlayerPosOnVerticalMove);
        return true;
    }
    return false;
}

bool ElectricWireRailKeeper::tryGetRailPointDestinationTrans(sead::Vector3f* out, int index) const {
    al::PlacementInfo& info = al::getRailPointInfo(this, index);
    return al::tryGetLinksTrans(out, info, "DestinationPoint");
}

bool ElectricWireRailKeeper::tryGetRailPointFastenerMoveLimitAreaFlag(int* out, int index) const {
    al::PlacementInfo& info = al::getRailPointInfo(this, index);
    return al::tryGetArg(out, info, "FastenerMoveLimitAreaFlag");
}

bool ElectricWireRailKeeper::tryGetRailPointOutDir(sead::Vector3f* out, int index) const {
    al::PlacementInfo& info = al::getRailPointInfo(this, index);
    al::PlacementInfo linksInfo{};
    if (!al::tryGetLinksInfo(&linksInfo, info, "DestinationPoint")) {
        bool isOutToRailPointDir = false;
        al::tryGetArg(&isOutToRailPointDir, info, "IsOutToRailPointDir");
        if (isOutToRailPointDir)
            return al::tryGetUp(out, info);
    } else {
        sead::Vector3f linksTrans;
        if (al::tryGetLinksTrans(&linksTrans, info, "DestinationPoint")) {
            sead::Vector3f railPointPos{};
            al::calcRailPointPos(&railPointPos, this, index);
            out->set(linksTrans);
            *out -= railPointPos;
            return al::tryNormalizeOrZero(out);
        }
    }
    return false;
}

void ElectricWireRailKeeper::endCameraIfActive() {
    al::CameraTicket* ticket = nullptr;
    for (s32 i = 0; i < this->mCameraTickets.size(); ++i) {
        ticket = this->mCameraTickets[i];
        if (al::isActiveCamera(ticket))
            al::endCamera(this->mElectricWire, ticket, -1, false);
    }
}

al::CameraPoser* ElectricWireRailKeeper::findRailPointCameraTicket(int priority) const {
    for (s32 i = 0; i < this->mCameraTickets.size(); ++i) {
        auto ticket = this->mCameraTickets[i];
        if (ticket->getPriority() == priority)
            return ticket->getPoser();
    }
    return nullptr;
}

const al::CameraTicketId*
ElectricWireRailKeeper::findRailPointStartCameraHackEndTicket(int priority) const {
    for (s32 i = 0; i < this->mCameraTickets.size(); ++i) {
        auto ticket = this->mCameraTickets[i];
        if (ticket->getPriority() == priority)
            return ticket->getTicketId();
    }
    return nullptr;
}

ElectricWireRailKeeper::ElectricWireRailKeeper(const char* name, al::LiveActor* wire)
    : LiveActor(name), mElectricWire(reinterpret_cast<ElectricWire*>(wire)) {}

ElectricWireRailKeeper::ElectricWireRailKeeper(const char* name) : LiveActor(name) {}

void ElectricWireRailKeeper::init(const al::ActorInitInfo& info) {
    using ElectricWireRailKeeperFunctor = al::FunctorV0M<ElectricWireRailKeeper*, void (ElectricWireRailKeeper::*)()>;

    al::initActorSceneInfo(this, info);
    al::initActorPoseTRSV(this);
    al::initActorSRT(this, info);
    al::initActorClipping(this, info);
    al::initStageSwitch(this, info);

    al::tryGetArg(&this->mIsShowLine, info, "IsShowLine");
    if (mElectricWire->mIsElectricWireRadio)
        this->mIsShowLine = false;
    al::tryGetArg(&this->mIsThrowaway, info, "IsThrowaway");
    if (al::tryGetLinksTrans(&this->mPlayerPosOnVerticalMove, info,
                             "PlayerHeadPosOnVerticalMove")) {
        this->mPosType = PosType::HEAD;
    } else if (al::tryGetLinksTrans(&this->mPlayerPosOnVerticalMove, info,
                                    "PlayerBottomPosOnVerticalMove")) {
        this->mPosType = PosType::BOTTOM;
    }
    if (!al::isExistRail(info, "Rail")) {
        this->kill();
        return;
    }
    this->initRailKeeper(info, "Rail");
    s32 pointNum = al::getRailPointNum(this);
    s32 needCameraPointNum = 0;
    for (s32 i = 0; i < pointNum; ++i) {
        bool isNeedCamera;
        al::tryGetArg(&isNeedCamera, al::getRailPointInfo(this, i), "IsNeedCamera");
        if (isNeedCamera) {
            ++needCameraPointNum;
            continue;
        }
        al::tryGetArg(&isNeedCamera, al::getRailPointInfo(this, i), "IsNeedStartCameraHackEnd");
        if (isNeedCamera) {
            ++needCameraPointNum;
            continue;
        }
    }
    this->mCameraTickets.allocBuffer(needCameraPointNum, nullptr);
    for (s32 i = 0; i < pointNum; ++i) {
        bool isNeedCamera;
        al::tryGetArg(&isNeedCamera, al::getRailPointInfo(this, i), "IsNeedCamera");
        al::CameraPoser* cameraPoser = nullptr;
        if (isNeedCamera) {
            sead::FixedSafeString<0x20> id;
            id.format("%d", i);
            cameraPoser =
                al::initObjectCamera(this->mElectricWire, info.getPlacementInfo(),
                                               id.mBuffer, nullptr);
        }
        al::tryGetArg(&isNeedCamera, al::getRailPointInfo(this, i), "IsNeedStartCameraHackEnd");
        al::CameraTicketId* ticketId;
        if (isNeedCamera) {
            sead::FixedSafeString<0x20> id;
            id.format("%d(Entrance)", i);
            ticketId = al::initEntranceCamera(this->mElectricWire, info.getPlacementInfo(), id.mBuffer);
        }
        if (cameraPoser != nullptr) {
            this->mCameraTickets.pushBack(new al::CameraTicket(cameraPoser, ticketId, i));
        }
    }
    al::initExecutorUpdate(this, info, "地形オブジェ[Movement]");
    al::initNerve(this, &Wait, 0);
    LiveActor::init(info);
    if (al::listenStageSwitchOnAppear(this, ElectricWireRailKeeperFunctor(this, &ElectricWireRailKeeper::appearBySwitch))) {
        this->initStageSwitchKeeper();
    }
    al::listenStageSwitchOnKill(this, ElectricWireRailKeeperFunctor(this, &ElectricWireRailKeeper::killBySwitch));
}
