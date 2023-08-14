#pragma once

#include <gfx/seadCamera.h>
#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/Nerve/NerveExecutor.h"
#include "Library/Yaml/ByamlIter.h"

struct CameraParam {
    bool gotMin;
    bool gotMax;
    f32 mMinFovyDegree;
    f32 mMaxFovyDegree;
};

namespace al {
class SnapShotCameraSceneInfo;
class ICameraInput;
class IUseCollision;

class SnapShotCameraCtrl : public NerveExecutor, public IUseAudioKeeper {
    SnapShotCameraSceneInfo* mCameraSceneInfo;
    CameraParam* mParam;
    bool mIsValidLookAtOffset;
    sead::Vector3f mLookAtOffset;
    sead::Vector3f vVar1;
    bool mIsValidZoomFovy;
    f32 mFovyDegree;
    f32 fVar2;
    f32 fVar3;
    f32 mMaxZoomOutFovyDegree;
    bool mIsValidRoll;
    f32 mRollDegree;
    f32 mRollTarget;
    u32 uVar2;
    bool bVar1;

public:
    SnapShotCameraCtrl(SnapShotCameraSceneInfo const*);
    void start(f32);
    void load(ByamlIter const&);
    void startReset(s32);
    void update(const sead::LookAtCamera&, const IUseCollision*, const ICameraInput*);
    void makeLookAtCameraPost(sead::LookAtCamera*) const;
    void makeLookAtCameraLast(sead::LookAtCamera*) const;
    void exeWait();
    void exeReset();

    f32 getFovyDegree() const { return mFovyDegree; };
};
}  // namespace al
