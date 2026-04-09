#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
}

class GolemClimbThrustPoint;
class GolemClimbWeakPoint;
class GolemJointIKRootCtrl;
class IUseDemoSkip;

class IUseGolemState {
public:
    virtual void updateLookAt() = 0;
    virtual void throwSearchBomb() = 0;
    virtual void throwReflectBomb() = 0;
    virtual void throwTsukkun() = 0;
    virtual void startDemo() = 0;
    virtual void endDemo() = 0;
    virtual void replaceDemoPlayer() = 0;
    virtual al::LiveActor* getActor() = 0;
    virtual al::LiveActor* getSkeleton() = 0;
    virtual void getShoutPos(sead::Vector3f*) const = 0;
    virtual GolemJointIKRootCtrl* getFootRootL() const = 0;
    virtual GolemJointIKRootCtrl* getFootRootR() const = 0;
    virtual GolemClimbThrustPoint* getThrustPointL() const = 0;
    virtual GolemClimbThrustPoint* getThrustPointR() const = 0;
    virtual GolemClimbWeakPoint* getWeakPoint(s32) const = 0;
    virtual bool isPrepareShout() const = 0;
    virtual void startShout() = 0;
    virtual void endShout() = 0;
    virtual IUseDemoSkip* getDemoSkip() = 0;
    virtual void stampFoot(const sead::Vector3f&, const sead::Quatf&) = 0;
    virtual void showDamageArea() = 0;
    virtual void hideDamageArea() = 0;
    virtual void setDamageAreaPose(const sead::Matrix34f&) = 0;
    virtual bool isInFootL() = 0;
    virtual bool isInFootR() = 0;
    virtual bool isMoon() = 0;
    virtual void updatePushSensor() = 0;
    virtual void endPushSensor() = 0;
    virtual bool tryNextPushSensor() const = 0;
};
