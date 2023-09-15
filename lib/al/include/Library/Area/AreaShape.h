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

    virtual bool isInVolume(const sead::Vector3f&) const = 0;
    virtual bool isInVolumeOffset(const sead::Vector3f&, f32) const = 0;
    virtual bool calcNearestEdgePoint(sead::Vector3f*, const sead::Vector3f&) const = 0;
    virtual bool checkArrowCollision(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&,
                                     const sead::Vector3f&) const = 0;
    virtual bool calcLocalBoundingBox(sead::BoundBox3f*) const = 0;

    sead::Vector3f getScale() const { return mScale; }

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
