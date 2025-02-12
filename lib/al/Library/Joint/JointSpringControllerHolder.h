#pragma once

#include <basis/seadTypes.h>

namespace al {
class ByamlIter;
class JointSpringController;
class LiveActor;

class JointSpringControllerHolder {
public:
    JointSpringControllerHolder();
    void init(s32);
    void init(LiveActor*, const char*);
    void init(LiveActor*, const ByamlIter&);
    void addController(JointSpringController*, const char*);
    void tryInvalidateConstraints(u32);
    void tryValidateConstraints(u32);
    void tryInvalidateConstraints(const char*);
    void tryValidateConstraints(const char*);
    void offControlAll();
    void setControlRateAll(f32);
    void onControlAll();
    void resetControlAll();
    void addControlRateAll(f32);
    void subControlRateAll(f32);
    void tryCreateAndInitJointControllerKeeper(LiveActor*, const char*);
    void calcInitFileSpringControlJointNum(const LiveActor*, const char*);

private:
    JointSpringController** mJointSpringControllerEntries;
    s32 mSize;
    s32 mMaxSize;
};

static_assert(sizeof(JointSpringControllerHolder) == 0x10);

}  // namespace al
