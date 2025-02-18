#pragma once

#include "Library/Camera/CameraPoserFix.h"

namespace al {

class CameraPoserFixActor : public CameraPoser {
public:
    CameraPoserFixActor(const LiveActor* actor);

    void init() override;
    void start(const CameraStartInfo& startInfo) override;
    void update() override;

private:
    const LiveActor* _140;
    char _148[28];
    sead::Vector3f mPreLookAtPos;
};

class CameraPoserFixTalk : public CameraPoserFixActor {
public:
    CameraPoserFixTalk(const LiveActor* actor);

    void start(const CameraStartInfo& startInfo) override;
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
static_assert(sizeof(CameraPoserFixTalk) == 0x170);

}  // namespace al
