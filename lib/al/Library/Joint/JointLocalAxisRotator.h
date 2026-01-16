#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Joint/JointControllerBase.h"

namespace al {

class JointLocalAxisRotator : public JointControllerBase {
public:
    JointLocalAxisRotator(const f32*, const sead::Vector3f&, bool);

    void calcJointCallback(s32, sead::Matrix34f*) override;
    const char* getCtrlTypeName() const override;

    void setVector28(const sead::Vector3f& v) { _28.set(v); }

private:
    sead::Vector3f _28;
    f32* _38;
    bool _40;
};

}  // namespace al
