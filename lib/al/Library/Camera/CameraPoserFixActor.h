#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class LiveActor;

class CameraPoserFixActor : public CameraPoser {
public:
    CameraPoserFixActor(const LiveActor* actor);

    void init() override;
    void start(const CameraStartInfo& startInfo) override;
    void update() override;

    void setTargetActor(const LiveActor* actor) { mActor = actor; }

    void setOffset(const sead::Vector3f& offset) { mOffset.set(offset); }

    void setDistance(f32 distance) { mDistance = distance; }

    void setAngleH(f32 angleH) { mAngleH = angleH; }

    void setAngleV(f32 angleV) { mAngleV = angleV; }

    void setAutoAroundFront() { mIsAutoAroundFront = true; }

    void setCalcNearestAtFromPreAt() { mIsCalcNearestAtFromPreAt = true; }

protected:
    const LiveActor* mActor;
    sead::Vector3f mOffset;
    f32 mDistance;
    f32 mAngleH;
    f32 mAngleV;
    bool mIsAutoAroundFront;
    bool mIsCalcNearestAtFromPreAt;
    u8 _162[2];
    sead::Vector3f mPreLookAtPos;
};

class CameraPoserFixTalk : public CameraPoserFixActor {
public:
    CameraPoserFixTalk(const LiveActor* actor);

    void start(const CameraStartInfo& startInfo) override;

    void setTalkDistance(f32 distance) { mTalkDistance = distance; }

private:
    f32 mTalkDistance;
    u8 _174[4];
};

class CameraPoserFixFishing : public CameraPoserFixActor {
public:
    CameraPoserFixFishing(const LiveActor* actor);

    void initParam(f32, const sead::Vector3f&, const sead::Vector3f&);
    void start(const CameraStartInfo& startInfo) override;

private:
    char _170[32];
};

static_assert(sizeof(CameraPoserFixActor) == 0x170);
static_assert(sizeof(CameraPoserFixFishing) == 0x190);
static_assert(sizeof(CameraPoserFixTalk) == 0x178);

}  // namespace al
