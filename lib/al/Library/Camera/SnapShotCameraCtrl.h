#pragma once

#include <gfx/seadCamera.h>

#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/Nerve/NerveExecutor.h"
#include "Library/Yaml/ByamlIter.h"

namespace al {
struct SnapShotCameraSceneInfo {
    void* unk;
    bool field_8;
    f32 field_C;
};
class ICameraInput;
class IUseCollision;

class SnapShotCameraCtrl : public NerveExecutor, public IUseAudioKeeper {
public:
    struct CameraParam {
        bool mHasMin;
        bool mHasMax;
        f32 mMinFovyDegree;
        f32 mMaxFovyDegree;
    };

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
    const SnapShotCameraSceneInfo* mCameraSceneInfo = nullptr;
    CameraParam* mParam = nullptr;
    bool mIsValidLookAtOffset = false;
    sead::Vector3f mLookAtOffset = sead::Vector3f(0.0f, 0.0f, 0.0f);
    sead::Vector3f field_38 = sead::Vector3f(0.0f, 0.0f, 0.0f);
    bool mIsValidZoomFovy = false;
    f32 mFovyDegree = 0.0f;
    f32 field_4C = 0.0f;
    f32 field_50 = 0.0f;
    f32 mMaxZoomOutFovyDegree = -1.0f;
    bool mIsValidRoll = false;
    f32 mRollDegree = false;
    f32 mRollTarget = false;
    u32 field_64 = -1;
    bool unk5 = false;
};
}  // namespace al
