#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class CameraTicket;
struct ActorInitInfo;
class HitSensor;
class JointLookAtController;
class JointSpringControllerHolder;
class SensorMsg;
}  // namespace al

class GoalMark;
class JangoCap;
class JangoStateBlowDown;
class JangoStateCapCatch;
class JangoStateCapGetDemo;
class JangoStateWaitTree;

class Jango : public al::LiveActor {
public:
    Jango(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;
    bool isEnableCap() const;
    void tryOffCapCatchedSwitch();
    void exeWaitCapGetDemo();
    void exeCapGetDemo();
    void exeEscape();
    void exeSurprise();
    void exeBlowDown();

private:
    JangoStateWaitTree* mStateWaitTree = nullptr;
    JangoStateCapGetDemo* mStateCapGetDemo = nullptr;
    JangoStateCapCatch* mStateCapCatch = nullptr;
    JangoStateBlowDown* mStateBlowDown = nullptr;
    s32 mDemoType = 0;
    sead::Vector3f mCapPlaceTrans = sead::Vector3f::zero;
    sead::Quatf mCapPlaceQuat = sead::Quatf::unit;
    JangoCap* mCap = nullptr;
    al::CameraTicket* mDemoCameraTicket = nullptr;
    al::CameraTicket* mEntranceCameraTicket = nullptr;
    al::JointLookAtController* mJointLookAtController = nullptr;
    bool mIsEnableCap = false;
    al::JointSpringControllerHolder* mJointSpringControllerHolder = nullptr;
    GoalMark* mGoalMark = nullptr;
    void* mDemoRequester = nullptr;
};

static_assert(sizeof(Jango) == 0x188);

namespace JangoFunction {
void setCapTransOnJoint(al::LiveActor* cap, al::LiveActor* jango);
void resetCapTransOnJoint(al::LiveActor* cap, al::LiveActor* jango);
void resetTransJointRoot(al::LiveActor* actor);
}  // namespace JangoFunction
