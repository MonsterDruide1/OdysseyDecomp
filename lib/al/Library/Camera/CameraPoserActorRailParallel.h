#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {
class CameraPoserActorRailParallel : public CameraPoser {
public:
    CameraPoserActorRailParallel(const char*, const RailKeeper* railKeeper);
    void init() override;
    void loadParam(const ByamlIter&) override;
    void start(const CameraStartInfo&) override;
    void update() override;

private:
    const RailKeeper* mRailKeeper;
    sead::Vector3f mOffset;
    f32 mDistance;
    f32 mAngleDegreeH;
    f32 mAngleDegreeV;
    f32 mFollowRate;
    f32 unk;
};
}  // namespace al
