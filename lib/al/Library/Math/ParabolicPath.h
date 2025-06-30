#pragma once

#include <math/seadVector.h>

namespace al {

class ParabolicPath {
public:
    ParabolicPath();

    void initFromUpVector(const sead::Vector3f& start, const sead::Vector3f& end,
                          const sead::Vector3f& up);
    void initFromUpVector(const sead::Vector3f& start, const sead::Vector3f& end,
                          const sead::Vector3f& up, f32 maxHeight);
    void initFromMaxHeight(const sead::Vector3f& start, const sead::Vector3f& end,
                           const sead::Vector3f& projectedEnd);
    void initFromUpVectorAddHeight(const sead::Vector3f& start, const sead::Vector3f& end,
                                   const sead::Vector3f& up, f32 height);

    f32 getLength(f32 start, f32 end, s32 iterations) const;
    f32 getTotalLength(s32 iterations) const;

    void calcPositionHV(sead::Vector3f* pos, f32 h, f32 v) const;
    void calcPosition(sead::Vector3f* pos, f32 prog) const;
    void calcPositionEaseOutH(sead::Vector3f* pos, f32 prog) const;
    void calcDirection(sead::Vector3f* dir, f32 prog, f32 stepSize) const;
    f32 calcPathSpeedFromGravityAccel(f32 frames) const;
    f32 calcPathSpeedFromAverageSpeed(f32 frames) const;
    f32 calcPathSpeedFromHorizontalSpeed(f32 frames) const;
    s32 calcPathTimeFromGravityAccel(f32 frames) const;
    s32 calcPathTimeFromAverageSpeed(f32 frames) const;
    s32 calcPathTimeFromHorizontalSpeed(f32 frames) const;

    const sead::Vector3f& getStart() const { return mStart; }

private:
    sead::Vector3f mStart = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mUp = {0.0f, 1.0f, 0.0f};
    sead::Vector3f mHorizontalDirection = {0.0f, 0.0f, 1.0f};
    f32 mGravity = 0.0f;
    f32 mInitialVelY = 0.0f;
    f32 mHorizontalDistance = 0.0f;
};

}  // namespace al
