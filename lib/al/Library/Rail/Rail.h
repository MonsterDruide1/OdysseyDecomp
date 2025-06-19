#pragma once

#include <math/seadVector.h>

#include "Library/Rail/RailPart.h"

namespace al {
class PlacementInfo;
class RailPart;

class Rail {
public:
    Rail();
    void init(const PlacementInfo& info);
    void calcPos(sead::Vector3f* pos, f32 distance) const;
    s32 getIncludedSection(const RailPart** part, f32* partDistance, f32 distance) const;
    void calcDirection(sead::Vector3f* direction, f32 distance) const;
    void calcPosDir(sead::Vector3f* position, sead::Vector3f* direction, f32 distance) const;
    f32 getTotalLength() const;
    f32 getPartLength(s32 index) const;
    f32 getLengthToPoint(s32 index) const;
    void calcRailPointPos(sead::Vector3f* pos, s32 index) const;
    void calcNearestRailPointPosFast(sead::Vector3f* rail_pos, u32* index,
                                     const sead::Vector3f& pos) const;
    void calcNearestRailPointNo(s32* index, const sead::Vector3f& pos) const;
    void calcNearestRailPointPos(sead::Vector3f* rail_pos, const sead::Vector3f& pos) const;
    f32 normalizeLength(f32 distance) const;
    f32 calcNearestRailPosCoord(const sead::Vector3f& pos, f32 interval) const;
    f32 calcNearestRailPosCoord(const sead::Vector3f& pos, f32 interval, f32* distance) const;
    f32 calcNearestRailPos(sead::Vector3f* rail_pos, const sead::Vector3f& pos, f32 interval) const;
    bool isNearRailPoint(f32 distance, f32 epsilon) const;
    s32 calcRailPointNum(f32 distance1, f32 distance2) const;
    f32 getIncludedSectionLength(f32* partDistance, f32* length, f32 distance) const;
    s32 getIncludedSectionIndex(f32 distance) const;
    bool isIncludeBezierRailPart() const;
    bool isBezierRailPart(s32 index) const;

private:
    PlacementInfo** mRailPoints = nullptr;
    RailPart* mRailPart = nullptr;
    s32 mRailPartCount = 0;
    s32 mRailPointsCount = 0;
    bool mIsClosed = false;
};

}  // namespace al
