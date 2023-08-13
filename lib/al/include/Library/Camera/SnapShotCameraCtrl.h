#pragma once

#include <gfx/seadCamera.h>
#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/Nerve/NerveExecutor.h"
#include "Library/Yaml/ByamlIter.h"

struct CameraParam {
    bool gotMin;
    bool gotMax;
    float mMinFovyDegree;
    float mMaxFovyDegree;
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
    float mFovyDegree;
    float fVar2;
    float fVar3;
    float mMaxZoomOutFovyDegree;
    bool mIsValidRoll;
    float mRollDegree;
    float mRollTarget;
    unsigned int uVar2;
    bool bVar1;

public:
    SnapShotCameraCtrl(SnapShotCameraSceneInfo const*);
    void start(float);
    void load(ByamlIter const&);
    void startReset(int);
    void update(const sead::LookAtCamera&, const IUseCollision*, const ICameraInput*);
    void makeLookAtCameraPost(sead::LookAtCamera*) const;
    void makeLookAtCameraLast(sead::LookAtCamera*) const;
    void exeWait();
    void exeReset();

    float getFovyDegree() const { return mFovyDegree; };
};
}  // namespace al
