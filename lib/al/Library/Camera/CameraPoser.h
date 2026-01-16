#pragma once

#include <gfx/seadCamera.h>
#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Area/IUseAreaObj.h"
#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/Collision/IUseCollision.h"
#include "Library/HostIO/HioNode.h"
#include "Library/HostIO/IUseName.h"
#include "Library/Nerve/IUseNerve.h"
#include "Library/Projection/OrthoProjectionInfo.h"
#include "Library/Rail/IUseRail.h"

namespace al {
class AudioDirector;
class ByamlIter;
class CameraAngleCtrlInfo;
class CameraAngleSwingInfo;
class CameraArrowCollider;
class CameraInputHolder;
class CameraOffsetCtrlPreset;
class CameraParamMoveLimit;
class CameraRequestParamHolder;
class CameraTargetAreaLimitter;
class CameraTargetCollideInfoHolder;
class CameraTargetHolder;
class CameraTurnInfo;
class CameraVerticalAbsorber;
class CameraViewInfo;
class GyroCameraCtrl;
class Nerve;
class PlacementInfo;
class RailKeeper;
class SnapShotCameraCtrl;
struct CameraFlagCtrl;
struct CameraObjectRequestInfo;
struct CameraPoserFlag;
struct CameraPoserSceneInfo;
struct CameraStartInfo;
struct OrthoProjectionInfo;
struct SnapShotCameraSceneInfo;

class CameraPoser : public HioNode,
                    public IUseAreaObj,
                    public IUseAudioKeeper,
                    public IUseCollision,
                    public IUseName,
                    public IUseNerve,
                    public IUseRail {
public:
    enum class ActiveState : s32 {
        Start = 0,
        Active = 1,
        End = 2,
    };

    enum class CameraInterpoleStepType : s32 {
        Undefined = -1,
        ByStep = 0,
        ByCameraDistance = 1,
    };

    struct LocalInterpole {
        inline void interpolate(sead::LookAtCamera* cam);

        s32 step = -1;
        s32 end = 0;
        sead::Vector3f prevCameraPos = {0, 0, 0};
        sead::Vector3f prevLookAtPos = {0, 0, 0};
    };

    static_assert(sizeof(LocalInterpole) == 0x20);

    struct LookAtInterpole {
        inline LookAtInterpole(f32 v) : _c(v) {}

        sead::Vector3f lookAtPos = {0.0f, 0.0f, 0.0f};
        f32 _c;
    };

    static_assert(sizeof(LookAtInterpole) == 0x10);

    struct CameraInterpoleStep {
        inline void load(const ByamlIter& iter);

        CameraInterpoleStepType stepType = CameraInterpoleStepType::Undefined;
        s32 stepNum = -1;
    };

    static_assert(sizeof(CameraInterpoleStep) == 0x8);

    struct CameraInterpoleParam : public CameraInterpoleStep {
        inline CameraInterpoleParam()
            : CameraInterpoleStep({CameraInterpoleStepType::ByCameraDistance}) {}

        inline void load(const ByamlIter& iter);
        inline void set(CameraInterpoleStepType type, s32 step, bool is_interpolate_by_step);

        s8 isEaseOut = false;
        bool isInterpolateByStep = false;
    };

    static_assert(sizeof(CameraInterpoleParam) == 0xC);

    struct OrthoProjectionParam {
        OrthoProjectionParam() {}

        inline void load(const ByamlIter& iter);

        bool isSetInfo = false;
        OrthoProjectionInfo info;
    };

    static_assert(sizeof(OrthoProjectionParam) == 0xC);

    CameraPoser(const char* name);
    AreaObjDirector* getAreaObjDirector() const override;

    virtual void init() {}

    virtual void initByPlacementObj(const PlacementInfo& info) {}

    virtual void endInit() {}

    virtual void start(const CameraStartInfo& info) {}

    virtual void update() {}

    virtual void end() { mActiveState = ActiveState::End; };

    virtual void loadParam(const ByamlIter& iter) {}

    virtual void makeLookAtCamera(sead::LookAtCamera* cam) const {}

    virtual bool receiveRequestFromObject(const CameraObjectRequestInfo& info) { return false; }

    virtual bool isZooming() const { return false; }

    virtual bool isEnableRotateByPad() const;

    virtual void startSnapShotMode() {}

    virtual void endSnapShotMode() {}

    const char* getName() const override { return mPoserName; }

    CollisionDirector* getCollisionDirector() const override;

    NerveKeeper* getNerveKeeper() const override { return mNerveKeeper; }

    AudioKeeper* getAudioKeeper() const override { return mAudioKeeper; }

    RailRider* getRailRider() const override;

    virtual void load(const ByamlIter& iter);
    virtual void movement();  // TODO: implementation missing
    virtual void calcCameraPose(sead::LookAtCamera* cam) const;

    virtual bool requestTurnToDirection(const CameraTurnInfo* info) { return false; }

    void appear(const CameraStartInfo& info);
    bool receiveRequestFromObjectCore(const CameraObjectRequestInfo& info);

    bool isInterpoleByCameraDistance() const;
    bool isInterpoleEaseOut() const;
    bool isEndInterpoleByStep() const;
    bool isFirstCalc() const;

    void initNerve(const Nerve* nerve, s32 maxStates);
    void initArrowCollider(CameraArrowCollider* arrowCollider);
    void initAudioKeeper(const char* name);
    void initRail(const PlacementInfo& info);
    void initLocalInterpole();
    // TODO: rename `v`
    void initLookAtInterpole(f32 v);
    void initOrthoProjectionParam();
    void tryInitAreaLimitter(const PlacementInfo& info);
    bool tryCalcOrthoProjectionInfo(OrthoProjectionInfo* projectionInfo) const;

    void makeLookAtCameraPrev(sead::LookAtCamera* cam) const;
    void makeLookAtCameraPost(sead::LookAtCamera* cam) const;
    void makeLookAtCameraLast(sead::LookAtCamera* cam) const;
    void makeLookAtCameraCollide(sead::LookAtCamera* cam) const;

    s32 getEndInterpoleStep() const;
    s32 getInterpoleStep() const;
    void setInterpoleStep(s32 step);
    void setInterpoleEaseOut();
    void resetInterpoleStep();

    void startSnapShotModeCore();
    void endSnapShotModeCore();

    f32 getFovyDegree() const;
    f32 getSceneFovyDegree() const;
    CameraInputHolder* getInputHolder() const;
    CameraTargetHolder* getTargetHolder() const;
    CameraFlagCtrl* getFlagCtrl() const;

    // get
    const sead::Vector3f& getPosition() const { return mPosition; };

    const sead::Vector3f& getTargetTrans() const { return mTargetTrans; };

    const sead::Vector3f& getCameraUp() const { return mCameraUp; };

    const sead::Matrix34f& getViewMtx() const { return mViewMtx; };

    bool is_98() const { return _98; }

    CameraViewInfo* getViewInfo() const { return mViewInfo; }

    // set
    void setPosition(const sead::Vector3f& vec) { mPosition.set(vec); };

    void setTargetTrans(const sead::Vector3f& vec) { mTargetTrans.set(vec); };

    void setCameraUp(const sead::Vector3f& vec) { mCameraUp.set(vec); };

    void setViewMtx(const sead::Matrix34f& mtx) { mViewMtx = mtx; }

    void setFovyDegree(f32 fovy) { mFovyDegree = fovy; }

protected:
    const char* mPoserName;
    ActiveState mActiveState = ActiveState::Start;
    sead::Vector3f mPosition = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mTargetTrans = {0.0f, 0.0f, 500.0f};
    sead::Vector3f mCameraUp = sead::Vector3f::ey;
    f32 mFovyDegree = 35.0f;
    f32 _64 = -1.0f;
    sead::Matrix34f mViewMtx = sead::Matrix34f::ident;
    bool _98 = false;
    CameraPoserSceneInfo* mSceneInfo = nullptr;
    CameraViewInfo* mViewInfo = nullptr;
    CameraPoserFlag* mPoserFlag;
    CameraVerticalAbsorber* mVerticalAbsorber = nullptr;
    CameraAngleCtrlInfo* mAngleCtrlInfo = nullptr;
    CameraAngleSwingInfo* mAngleSwingInfo = nullptr;
    CameraArrowCollider* mArrowCollider = nullptr;
    CameraOffsetCtrlPreset* mOffsetCtrlPreset = nullptr;
    LocalInterpole* mLocalInterpole = nullptr;
    LookAtInterpole* mLookAtInterpole = nullptr;
    CameraParamMoveLimit* mParamMoveLimit = nullptr;
    CameraTargetAreaLimitter* mTargetAreaLimitter = nullptr;
    GyroCameraCtrl* mGyroCtrl = nullptr;
    SnapShotCameraCtrl* mSnapShotCtrl = nullptr;
    AudioKeeper* mAudioKeeper = nullptr;
    NerveKeeper* mNerveKeeper = nullptr;
    RailKeeper* mRailKeeper = nullptr;
    CameraInterpoleParam* mActiveInterpoleParam = nullptr;
    CameraInterpoleStep* mEndInterpoleParam = nullptr;
    OrthoProjectionParam* mOrthoProjectionParam = nullptr;
};

static_assert(sizeof(CameraPoser) == 0x140);

}  // namespace al
