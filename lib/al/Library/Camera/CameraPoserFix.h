#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserFix : public CameraPoser {
public:
    CameraPoserFix(const char*);

    static const char* getFixAbsoluteCameraName();
    static const char* getFixDoorwayCameraName();

    void initCameraPosAndLookAtPos(const sead::Vector3f& cameraPos,
                                   const sead::Vector3f& lookAtPos);

    void init() override;
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& startInfo) override;
    void update() override;

private:
    sead::Vector3f mLookAtPos;
    f32 mDistance;
    f32 mAngleV;
    f32 mAngleH;
    bool mIsCalcNearestAtFromPreAt;
    f32 mPreLookAtPos;
    void* _160;
};

static_assert(sizeof(CameraPoserFix) == 0x168);

class LiveActor;

class CameraPoserFixActor : public CameraPoser {
public:
    CameraPoserFixActor(const LiveActor* actor);

    void init() override;
    void start(const CameraStartInfo& startInfo) override;
    void update() override;

private:
};

class CameraPoserFixTalk : public CameraPoserFixActor {
public:
    CameraPoserFixTalk(const LiveActor* actor);

    void start(const CameraStartInfo& startInfo) override;

private:
};

class CameraPoserFixFishing : public CameraPoserFixActor {
public:
    CameraPoserFixFishing(const LiveActor* actor);

    void initParam(f32, const sead::Vector3f&, const sead::Vector3f&);
    void start(const CameraStartInfo& startInfo) override;

private:
};

class CameraPoserFixLook : public CameraPoser {
public:
    CameraPoserFixLook(const LiveActor* actor);

    void init() override;
    void start(const CameraStartInfo& startInfo) override;

    sead::Vector3f* getVec() { return _0; }

private:
    sead::Vector3f* _0;
};

static_assert(sizeof(CameraPoserFixLook) == 0x148);

class CameraPoserFixPoint : public CameraPoserFix {
public:
    CameraPoserFixPoint(const LiveActor* actor);

    void init() override;
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& startInfo) override;
    void update() override;
    void makeLookAtCamera(sead::LookAtCamera* lookAtCam) const override;

private:
};

}  // namespace al
