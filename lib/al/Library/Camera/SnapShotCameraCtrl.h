#pragma once

#include <gfx/seadCamera.h>

#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/Nerve/NerveExecutor.h"
#include "Library/Yaml/ByamlIter.h"

struct CameraParam {
    bool hasMin;
    bool hasMax;
    f32 minFovyDegree;
    f32 maxFovyDegree;
};

namespace al {
struct SnapShotCameraSceneInfo;
class ICameraInput;
class IUseCollision;

class SnapShotCameraCtrl : public NerveExecutor, public IUseAudioKeeper {
public:
    SnapShotCameraCtrl(const SnapShotCameraSceneInfo*);
    void start(f32);
    void load(const ByamlIter&);
    void startReset(s32);
    void update(const sead::LookAtCamera&, const IUseCollision*, const ICameraInput*);
    void makeLookAtCameraPost(sead::LookAtCamera*) const;
    void makeLookAtCameraLast(sead::LookAtCamera*) const;
    void exeWait();
    void exeReset();

    f32 getFovyDegree() const { return mFovyDegree; };

private:
    SnapShotCameraSceneInfo* mCameraSceneInfo;
    CameraParam* mParam;
    bool mIsValidLookAtOffset;
    sead::Vector3f mLookAtOffset;
    sead::Vector3f unk1;
    bool mIsValidZoomFovy;
    f32 mFovyDegree;
    f32 unk2;
    f32 unk3;
    f32 mMaxZoomOutFovyDegree;
    bool mIsValidRoll;
    f32 mRollDegree;
    f32 mRollTarget;
    u32 unk4;
    bool unk5;
};
}  // namespace al
