#pragma once

#include "Library/Area/AreaShape.h"

namespace al {
class AreaShapeCube : public AreaShape {
public:
    enum class OriginType : s32 { Center, Base, Top };

    AreaShapeCube(AreaShapeCube::OriginType originType);

    bool isInVolume(const sead::Vector3f& trans) const override;
    bool isInVolumeOffset(const sead::Vector3f& trans, f32 offset) const override;
    bool calcNearestEdgePoint(sead::Vector3f* out, const sead::Vector3f& trans) const override;
    bool checkArrowCollision(sead::Vector3f* outTrans, sead::Vector3f* outDir,
                             const sead::Vector3f& pos1, const sead::Vector3f& pos2) const override;
    bool calcLocalBoundingBox(sead::BoundBox3f* out) const override;

    bool isInLocalVolume(const sead::Vector3f& trans) const;

    f32 calcBottom() const {
        if (mOriginType == OriginType::Base)
            return 0.0f;
        else if (mOriginType == OriginType::Top)
            return -1000.0f;
        else
            return -500.0f;
    }

    f32 calcTop() const {
        if (mOriginType == OriginType::Base)
            return 1000.0f;
        else if (mOriginType == OriginType::Top)
            return 0.0f;
        else
            return 500.0f;
    }

private:
    AreaShapeCube::OriginType mOriginType;
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

class AreaShapeSphere : public AreaShape {
public:
    AreaShapeSphere();

    bool isInVolume(const sead::Vector3f& trans) const override;
    bool isInVolumeOffset(const sead::Vector3f& trans, f32 offset) const override;
    bool calcNearestEdgePoint(sead::Vector3f* out, const sead::Vector3f& trans) const override;
    bool checkArrowCollision(sead::Vector3f* outTrans, sead::Vector3f* outDir,
                             const sead::Vector3f& pos1, const sead::Vector3f& pos2) const override;
    bool calcLocalBoundingBox(sead::BoundBox3f* out) const override;
};

class AreaShapeOval : public AreaShape {
public:
    AreaShapeOval();

    bool isInVolume(const sead::Vector3f& trans) const override;
    bool isInVolumeOffset(const sead::Vector3f& trans, f32 offset) const override;
    bool calcNearestEdgePoint(sead::Vector3f* out, const sead::Vector3f& trans) const override;
    bool checkArrowCollision(sead::Vector3f* outTrans, sead::Vector3f* outDir,
                             const sead::Vector3f& pos1, const sead::Vector3f& pos2) const override;
    bool calcLocalBoundingBox(sead::BoundBox3f* out) const override;
};

class AreaShapeCylinder : public AreaShape {
public:
    enum class OriginType : s32 { Center, Base, Top };

    AreaShapeCylinder(AreaShapeCylinder::OriginType originType);

    bool isInVolume(const sead::Vector3f& trans) const override;
    bool isInVolumeOffset(const sead::Vector3f& trans, f32 offset) const override;
    bool calcNearestEdgePoint(sead::Vector3f* out, const sead::Vector3f& trans) const override;
    bool checkArrowCollision(sead::Vector3f* outTrans, sead::Vector3f* outDir,
                             const sead::Vector3f& pos1, const sead::Vector3f& pos2) const override;
    bool calcLocalBoundingBox(sead::BoundBox3f* out) const override;

private:
    AreaShapeCylinder::OriginType mOriginType;
};

class AreaShapeCylinderBase : public AreaShapeCylinder {
public:
    AreaShapeCylinderBase() : AreaShapeCylinder(AreaShapeCylinder::OriginType::Base) {}
};

class AreaShapeCylinderCenter : public AreaShapeCylinder {
public:
    AreaShapeCylinderCenter() : AreaShapeCylinder(AreaShapeCylinder::OriginType::Center) {}
};

class AreaShapeCylinderTop : public AreaShapeCylinder {
public:
    AreaShapeCylinderTop() : AreaShapeCylinder(AreaShapeCylinder::OriginType::Top) {}
};

class AreaShapeInfinite : public AreaShape {
public:
    AreaShapeInfinite();

    bool isInVolume(const sead::Vector3f& trans) const override;
    bool isInVolumeOffset(const sead::Vector3f& trans, f32 offset) const override;
    bool calcNearestEdgePoint(sead::Vector3f* out, const sead::Vector3f& trans) const override;
    bool checkArrowCollision(sead::Vector3f* outTrans, sead::Vector3f* outDir,
                             const sead::Vector3f& pos1, const sead::Vector3f& pos2) const override;
    bool calcLocalBoundingBox(sead::BoundBox3f* out) const override;
};
}  // namespace al
