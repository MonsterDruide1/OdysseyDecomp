#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Joint/JointControllerBase.h"

namespace al {
class ByamlIter;
class LiveActor;

class JointSpringController : public JointControllerBase {
public:
    JointSpringController();

    void setChildLocalPos(const sead::Vector3f&);
    void setChildLocalMtxPtr(const sead::Matrix34f*);
    void setStability(f32);
    void setFriction(f32);
    void setLimitDegree(f32);
    void setControlRate(f32);
    void addControlRate(f32);
    void subControlRate(f32);
    void setJointName(const char*);
    void load(const ByamlIter&);
    void reset();
    void calcChildPos(sead::Vector3f*, const sead::Matrix34f*) const;
    bool canCompute(sead::Matrix34f*, sead::Vector3f*);
    void calcJointCallback(s32, sead::Matrix34f*);
    void compute(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&,
                 const sead::Vector3f&);
    const char* getCtrlTypeName() const;

private:
    char* filler[0xe0];
};

}  // namespace al
