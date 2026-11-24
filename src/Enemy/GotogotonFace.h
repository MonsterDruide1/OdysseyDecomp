#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Enemy/GotogotonEdge.h"

class GotogotonFace {
public:
    GotogotonFace();

    GotogotonEdge* getEdge(s32 index);
    const GotogotonEdge* getEdge(s32 index) const;
    const GotogotonFace* getNextFace(s32 index) const;

    void setOppositeFace(const GotogotonFace* face);
    void setCenter(const sead::Vector3f& vector);
    void setNormal(const sead::Vector3f& vector);

    s32 calcNearEdgeIndexToPos(const sead::Vector3f& pos, const sead::Quatf& quat,
                               const sead::Vector3f& trans) const;
    s32 calcNearEdgeIndexToDir(const sead::Vector3f& dir, const sead::Quatf& quat,
                               const sead::Vector3f& trans) const;
    s32 calcOppositeEdgeIndex(s32 edgeIndex) const;
    void calcRotateCenterPos(sead::Vector3f* outPos, s32 edgeIndex, const sead::Quatf& quat,
                             const sead::Vector3f& trans, f32 degree) const;
    void calcLandEffectMtx(sead::Matrix34f* outMtx, const sead::Quatf& quat,
                           const sead::Vector3f& trans, s32 edgeIndex) const;
    void calcLandEffectMtxNextFace(sead::Matrix34f* outMtx, const sead::Quatf& quat,
                                   const sead::Vector3f& trans, s32 edgeIndex) const;
    void calcCenterPos(sead::Vector3f* outPos, const sead::Quatf& quat,
                       const sead::Vector3f& trans) const;
    void calcCenterDir(sead::Vector3f* outDir, const sead::Quatf& quat) const;

    void rotate(sead::Quatf* outQuat, sead::Vector3f* outTrans, s32 edgeIndex,
                const sead::Quatf& quat, const sead::Vector3f& trans, float _unk) const;

    GotogotonEdge* mEdges = new GotogotonEdge[4]();
    const GotogotonFace* mOppositeFace = nullptr;
    sead::Vector3f mCenter = sead::Vector3f::zero;
    sead::Vector3f mNormal = sead::Vector3f::ey;
};
