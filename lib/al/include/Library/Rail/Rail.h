#pragma once

#include <math/seadVector.h>
#include "Library/Rail/RailPart.h"

namespace al {
class PlacementInfo;
class RailPart;

class Rail {
public:
    Rail();
    void init(const PlacementInfo&);
    void calcPos(sead::Vector3f*, f32) const;
    s32 getIncludedSection(const RailPart**, f32*, f32) const;
    void calcDirection(sead::Vector3f*, f32) const;
    void calcPosDir(sead::Vector3f*, sead::Vector3f*, f32) const;
    f32 getTotalLength() const;
    f32 getPartLength(s32) const;
    f32 getLengthToPoint(s32) const;
    void calcRailPointPos(sead::Vector3f*, s32) const;
    void calcNearestRailPointPosFast(sead::Vector3f*, u32*, const sead::Vector3f&) const;
    void calcNearestRailPointNo(s32*, const sead::Vector3f&) const;
    void calcNearestRailPointPos(sead::Vector3f*, const sead::Vector3f&) const;
    f32 normalizeLength(f32) const;
    f32 calcNearestRailPosCoord(const sead::Vector3f&, f32) const;
    f32 calcNearestRailPosCoord(const sead::Vector3f&, f32, f32*) const;
    f32 calcNearestRailPos(sead::Vector3f*, const sead::Vector3f&, f32) const;
    bool isNearRailPoint(f32, f32) const;
    s32 calcRailPointNum(f32, f32) const;
    f32 getIncludedSectionLength(f32*, f32*, f32) const;
    s32 getIncludedSectionIndex(f32) const;
    bool isIncludeBezierRailPart() const;
    bool isBezierRailPart(s32) const;

private:
    PlacementInfo** mRailPoints = nullptr;
    RailPart* mRailPart = nullptr;
    s32 mRailPartCount = 0;
    s32 mRailPointsCount = 0;
    bool isClosed = false;
};

}  // namespace al
