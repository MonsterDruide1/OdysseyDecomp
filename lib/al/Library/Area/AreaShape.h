#pragma once

#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class AreaShape : public HioNode {
public:
    AreaShape();

    virtual bool isInVolume(const sead::Vector3f& trans) const = 0;
    virtual bool isInVolumeOffset(const sead::Vector3f& trans, f32 offset) const = 0;
    virtual bool calcNearestEdgePoint(sead::Vector3f* out, const sead::Vector3f& trans) const = 0;
    virtual bool checkArrowCollision(sead::Vector3f* outPos, sead::Vector3f* outNormal,
                                     const sead::Vector3f& pos1,
                                     const sead::Vector3f& pos2) const = 0;
    virtual bool calcLocalBoundingBox(sead::BoundBox3f* out) const = 0;

    void setBaseMtxPtr(const sead::Matrix34f* baseMtxPtr);
    void setScale(const sead::Vector3f& scale);
    bool calcLocalPos(sead::Vector3f* localPos, const sead::Vector3f& trans) const;
    bool calcWorldPos(sead::Vector3f* worldPos, const sead::Vector3f& trans) const;
    bool calcWorldDir(sead::Vector3f* worldDir, const sead::Vector3f& trans) const;
    void calcTrans(sead::Vector3f* trans) const;

    const sead::Vector3f& getScale() const { return mScale; }

private:
    const sead::Matrix34f* mBaseMtxPtr = nullptr;
    sead::Vector3f mScale = {1.0f, 1.0f, 1.0f};
};
}  // namespace al
