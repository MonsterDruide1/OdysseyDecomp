#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

class GotogotonMark;
class GotogotonEdge;

class GotogotonFace {
public:
    GotogotonFace();

    GotogotonEdge* getEdge(s32);
    const GotogotonEdge* getEdge(s32) const;
    const GotogotonFace* getNextFace(s32) const;
    void setOppositeFace(const GotogotonFace*);
    void setCenter(const sead::Vector3f&);
    void setNormal(const sead::Vector3f&);
    s32 calcNearEdgeIndexToPos(const sead::Vector3f&, const sead::Quatf&,
                               const sead::Vector3f&) const;
    s32 calcNearEdgeIndexToDir(const sead::Vector3f&, const sead::Quatf&,
                               const sead::Vector3f&) const;
    void rotate(sead::Quatf*, sead::Vector3f*, s32, const sead::Quatf&, const sead::Vector3f&,
                f32) const;
    s32 calcOppositeEdgeIndex(s32) const;
    void calcRotateCenterPos(sead::Vector3f*, s32, const sead::Quatf&, const sead::Vector3f&,
                             f32) const;
    void calcLandEffectMtx(sead::Matrix34f*, const sead::Quatf&, const sead::Vector3f&, s32) const;
    void calcLandEffectMtxNextFace(sead::Matrix34f*, const sead::Quatf&, const sead::Vector3f&,
                                   s32) const;
    void calcCenterPos(sead::Vector3f*, const sead::Quatf&, const sead::Vector3f&) const;
    void calcCenterDir(sead::Vector3f*, const sead::Quatf&) const;

private:
    char _0[0x28];
};
