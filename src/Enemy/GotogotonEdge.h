#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

class GotogotonFace;

class GotogotonEdge {
public:
    GotogotonEdge();

    void setTrans(const sead::Vector3f&);
    void setAxis(const sead::Vector3f&);
    void setNextFace(const GotogotonFace*);
    void calcAxisAndPos(sead::Vector3f*, sead::Vector3f*, const sead::Quatf&,
                        const sead::Vector3f&) const;
};
