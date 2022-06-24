#pragma once

#include <math/seadVector.h>
#include "al/rail/RailPart.h"

namespace al {
class PlacementInfo;
class RailPart;

class Rail {
public:
    Rail();
    void init(const PlacementInfo&);
    void calcPos(sead::Vector3f*, float) const;
    int getIncludedSection(const RailPart**, float*, float) const;
    void calcDirection(sead::Vector3f*, float) const;
    void calcPosDir(sead::Vector3f*, sead::Vector3f*, float) const;
    float getTotalLength() const;
    float getPartLength(int) const;
    float getLengthToPoint(int) const;
    void calcRailPointPos(sead::Vector3f*, int) const;
    void calcNearestRailPointPosFast(sead::Vector3f*, u32*, const sead::Vector3f&) const;
    void calcNearestRailPointNo(int*, const sead::Vector3f&) const;
    void calcNearestRailPointPos(sead::Vector3f*, const sead::Vector3f&) const;
    float normalizeLength(float) const;
    float calcNearestRailPosCoord(const sead::Vector3f&, float) const;
    float calcNearestRailPosCoord(const sead::Vector3f&, float, float*) const;
    float calcNearestRailPos(sead::Vector3f*, const sead::Vector3f&, float) const;
    bool isNearRailPoint(float, float) const;
    int calcRailPointNum(float, float) const;
    float getIncludedSectionLength(float*, float*, float) const;
    int getIncludedSectionIndex(float) const;
    bool isIncludeBezierRailPart() const;
    bool isBezierRailPart(int) const;

private:
    PlacementInfo** mRailPoints = nullptr;
    RailPart* mRailPart = nullptr;
    int mRailPartCount = 0;
    int mRailPointsCount = 0;
    bool isClosed = false;
};

}  // namespace al
