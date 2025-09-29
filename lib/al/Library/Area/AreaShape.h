#pragma once

#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Factory/Factory.h"
#include "Library/HostIO/HioNode.h"

namespace al {

class AreaShape : public HioNode {
public:
    AreaShape();

    virtual bool isInVolume(const sead::Vector3f& pos) const = 0;
    virtual bool isInVolumeOffset(const sead::Vector3f& pos, f32 offset) const = 0;
    virtual bool calcNearestEdgePoint(sead::Vector3f* edgePoint,
                                      const sead::Vector3f& pos) const = 0;
    // TODO: rename parameters
    virtual bool checkArrowCollision(sead::Vector3f* a2, sead::Vector3f* a3,
                                     const sead::Vector3f& a4, const sead::Vector3f& a5) const = 0;
    virtual bool calcLocalBoundingBox(sead::BoundBox3f* boundingBox) const = 0;

    const sead::Vector3f& getScale() const { return mScale; }

    void setBaseMtxPtr(const sead::Matrix34f* baseMtxPtr);
    void setScale(const sead::Vector3f& scale);

    bool calcLocalPos(sead::Vector3f* localPos, const sead::Vector3f& trans) const;
    bool calcWorldPos(sead::Vector3f* worldPos, const sead::Vector3f& trans) const;
    bool calcWorldDir(sead::Vector3f* worldDir, const sead::Vector3f& trans) const;
    void calcTrans(sead::Vector3f* trans) const;

private:
    const sead::Matrix34f* mBaseMtxPtr = nullptr;
    sead::Vector3f mScale = {1.0f, 1.0f, 1.0f};
};

using AreaShapeCreatorFunction = AreaShape* (*)();

class AreaShapeFactory : public Factory<AreaShapeCreatorFunction> {
public:
    AreaShapeFactory(const char* factoryName);
};

}  // namespace al
