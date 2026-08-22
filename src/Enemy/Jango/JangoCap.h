#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Event/IEventFlowEventReceiver.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {
class CameraTicket;
struct ActorInitInfo;
class EventFlowEventData;
class EventFlowExecutor;
class HitSensor;
class SensorMsg;
}  // namespace al

class BalloonIcon;
class Jango;
class JangoCapSeparateHelpClaimUpdater;
class StateStageTalkDemoCapManHero;

class JangoCap : public al::LiveActor, public al::IEventFlowEventReceiver {
public:
    JangoCap(const char* name, s32 demoType, Jango* jango);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    bool receiveEvent(const al::EventFlowEventData* event) override;
    void control() override;
    void demoEnd();
    void hideBalloon();
    void startBlowDown();
    void released(const sead::Vector3f& trans, const sead::Quatf& quat);
    void catched();
    void directCatched();
    void startStruggle();
    void exeCatched();
    void exeCarried();
    void exeReleaseDemoWait();
    void exeFall();
    void exeWait();
    void exeTalkDemo();

private:
    Jango* mJango = nullptr;
    BalloonIcon* mBalloonIcon = nullptr;
    sead::Vector3f mReleaseTrans = sead::Vector3f::zero;
    sead::Quatf mReleaseQuat = sead::Quatf::unit;
    sead::Vector3f mTalkDemoPlayerTrans = sead::Vector3f::zero;
    sead::Quatf mTalkDemoPlayerQuat = sead::Quatf::unit;
    al::EventFlowExecutor* mEventFlowExecutor = nullptr;
    StateStageTalkDemoCapManHero* mStateTalkDemo = nullptr;
    JangoCapSeparateHelpClaimUpdater* mSeparateHelpClaimUpdater = nullptr;
    al::CameraTicket* mEntranceCameraTicket = nullptr;
    bool mIsBlowDown = false;
};

static_assert(sizeof(JangoCap) == 0x180);
