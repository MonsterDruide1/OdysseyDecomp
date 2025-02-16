#pragma once

#include <basis/seadTypes.h>

namespace al {

class ByamlIter;
class LiveActor;
class JointSpringController;

class JointSpringControllerHolder {
public:
    JointSpringControllerHolder();
    void init(s32);
    void init(LiveActor*, const char*);
    void init(LiveActor*, const ByamlIter&);
    void addController(JointSpringController*, const char*);
    bool tryInvalidateConstraints(u32);
    bool tryValidateConstraints(u32);
    bool tryInvalidateConstraints(const char*);
    bool tryValidateConstraints(const char*);
    void offControlAll();
    void setControlRateAll(f32);
    void onControlAll();
    void resetControlAll();
    void addControlRateAll(f32);
    void subControlRateAll(f32);
    JointSpringControllerHolder* tryCreateAndInitJointControllerKeeper(LiveActor*, const char*);
    s32 calcInitFileSpringControlJointNum(const LiveActor*, const char*);

private:
    void* field_0;
    void* field_8;
};

}  // namespace al
