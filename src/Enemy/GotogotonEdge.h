#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

class GotogotonFace;

class GotogotonEdge {
public:
    GotogotonEdge();

    void setTrans(const sead::Vector3f& trans);
    void setAxis(const sead::Vector3f& axis);
    void setNextFace(const GotogotonFace* face);
    void calcAxisAndPos(sead::Vector3f* outAxis, sead::Vector3f* outPos, const sead::Quatf& quat,
                        const sead::Vector3f& trans) const;

    const sead::Vector3f& getAxis() { return mAxis; }

    const GotogotonFace* getNextFace() { return mNextFace; }

private:
    sead::Vector3f mTrans = sead::Vector3f::zero;
    sead::Vector3f mAxis = sead::Vector3f::ex;
    const GotogotonFace* mNextFace = nullptr;
};

static_assert(sizeof(GotogotonEdge) == 0x20);
