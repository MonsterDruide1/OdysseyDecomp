#pragma once

#include "Library/Area/AreaShape.h"

namespace al {

class AreaShapeCube : public AreaShape {
public:
    enum class OriginType { Center, Base, Top };

    AreaShapeCube(OriginType originType);

    bool isInVolume(const sead::Vector3f& pos) const override;
    bool isInVolumeOffset(const sead::Vector3f& pos, f32 offset) const override;
    bool calcNearestEdgePoint(sead::Vector3f* edgePoint, const sead::Vector3f& pos) const override;
    bool checkArrowCollision(sead::Vector3f* a2, sead::Vector3f* a3, const sead::Vector3f& a4,
                             const sead::Vector3f& a5) const override;
    bool calcLocalBoundingBox(sead::BoundBox3f* boundingBox) const override;

    bool isInLocalVolume(const sead::Vector3f& pos) const;

private:
    OriginType mOriginType;
};

class AreaShapeCubeBase : public AreaShapeCube {
public:
    AreaShapeCubeBase() : AreaShapeCube(AreaShapeCube::OriginType::Base) {}
};

class AreaShapeCubeCenter : public AreaShapeCube {
public:
    AreaShapeCubeCenter() : AreaShapeCube(AreaShapeCube::OriginType::Center) {}
};

class AreaShapeCubeTop : public AreaShapeCube {
public:
    AreaShapeCubeTop() : AreaShapeCube(AreaShapeCube::OriginType::Top) {}
};

}  // namespace al
