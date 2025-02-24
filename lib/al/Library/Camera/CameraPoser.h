#pragma once

#include <gfx/seadCamera.h>
#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Area/IUseAreaObj.h"
#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/Camera/CameraArrowCollider.h"
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
class CameraFlagCtrl;
class CameraInputHolder;
struct CameraObjectRequestInfo;
class CameraOffsetCtrlPreset;
class CameraParamMoveLimit;
struct CameraPoserFlag;
struct CameraStartInfo;
class CameraTargetAreaLimitter;
class CameraTargetHolder;
class CameraTurnInfo;
class CameraVerticalAbsorber;
class CameraViewInfo;
class GyroCameraCtrl;
class Nerve;
struct OrthoProjectionInfo;
class PlacementInfo;
class RailKeeper;
class SnapShotCameraCtrl;

class CameraPoser : public HioNode,
                    public IUseAreaObj,
                    public IUseAudioKeeper,
                    public IUseCollision,
                    public IUseName,
                    public IUseNerve,
                    public IUseRail {
public:
    enum ActiveState {
        ActiveState_Start = 0,
        ActiveState_Run = 1,
        ActiveState_End = 2,
    };

    enum CameraInterpoleStepType {
        CameraInterpoleStepType_Undefined = -1,
        CameraInterpoleStepType_ByStep = 0,
        CameraInterpoleStepType_ByCameraDistance = 1,
    };

    struct LocalInterpole {
        void interpolate(sead::LookAtCamera* cam);

        s32 _0 = -1;
        s32 _4 = 0;
        sead::Vector3f mCameraPos = {0, 0, 0};
        sead::Vector3f mLookAtPos = {0, 0, 0};
    };

    static_assert(sizeof(LocalInterpole) == 0x20);

    struct LookAtInterpole {
        LookAtInterpole(f32 v) : _c(v) {}

        sead::Vector3f mLookAtPos = {0.0f, 0.0f, 0.0f};
        f32 _c;
    };

    static_assert(sizeof(LookAtInterpole) == 0x10);

    struct CameraInterpolateStep {
        CameraInterpolateStep(CameraInterpoleStepType type, s32 num)
            : mStepType(type), mStepNum(num) {}

        void load(const ByamlIter& iter);

        CameraInterpoleStepType mStepType;
        s32 mStepNum;
    };

    static_assert(sizeof(CameraInterpolateStep) == 0x8);

    struct CameraInterpolateParam {
        CameraInterpolateParam(CameraInterpoleStepType type, s32 num, s8 isEaseOut, s8 isInterpol)
            : mStep(type, num), mIsEaseOut(isEaseOut), mIsInterpolate(isInterpol) {}

        void load(const ByamlIter& iter);
        void set(CameraInterpoleStepType type, s32 step, bool isInterpolate);

        CameraInterpolateStep mStep;
        s8 mIsEaseOut;
        s8 mIsInterpolate;
    };

    static_assert(sizeof(CameraInterpolateParam) == 0xC);

    struct OrthoProjectionParam {
        OrthoProjectionParam(bool isSetInfo, OrthoProjectionInfo info)
            : mIsSetInfo(isSetInfo), mInfo(info) {}

        void load(const ByamlIter& iter);

        bool mIsSetInfo;
        OrthoProjectionInfo mInfo;
    };

    static_assert(sizeof(OrthoProjectionParam) == 0xC);

    struct CameraPoserControls {
        f32 mSceneFovyDegree;
        AreaObjDirector* mAreaObjDirector;
        CollisionDirector* mCollisionDirector;
        AudioDirector* mAudioDirector;
        CameraInputHolder* mInputHolder;
        CameraTargetHolder* mTargetHolder;
        CameraFlagCtrl* mFlagCtrl;
    };

    CameraPoser(const char* poserName);
    virtual AreaObjDirector* getAreaObjDirector() const override;
    virtual void init();
    virtual void initByPlacementObj(const PlacementInfo&);
    virtual void endInit();
    virtual void start(const CameraStartInfo&);
    virtual void update();
    virtual void end();
    virtual void loadParam(const ByamlIter&);
    virtual void makeLookAtCamera(sead::LookAtCamera*) const;
    virtual bool receiveRequestFromObject(const CameraObjectRequestInfo&);
    virtual bool isZooming() const;
    virtual bool isEnableRotateByPad() const;
    virtual void startSnapShotMode();
    virtual void endSnapShotMode();

    virtual const char* getName() const override;
    virtual CollisionDirector* getCollisionDirector() const override;
    virtual NerveKeeper* getNerveKeeper() const override;
    virtual AudioKeeper* getAudioKeeper() const override;
    virtual RailRider* getRailRider() const override;

    virtual void load(const ByamlIter& iter);
    virtual void movement();
    virtual void calcCameraPose(sead::LookAtCamera* cam) const;
    virtual bool requestTurnToDirection(const CameraTurnInfo*);

    void appear(const CameraStartInfo& startInfo);
    bool receiveRequestFromObjectCore(const CameraObjectRequestInfo& objRequestInfo);

    bool isInterpoleByCameraDistance() const;
    bool isInterpoleEaseOut() const;
    bool isEndInterpoleByStep() const;
    bool isFirstCalc() const;

    void initNerve(const Nerve* nerve, s32 maxStates);
    void initArrowCollider(CameraArrowCollider* arrowCollider);
    void initAudioKeeper(const char* name);
    void initRail(const PlacementInfo& placementInfo);
    void initLocalInterpole();
    void initLookAtInterpole(f32);
    void initOrthoProjectionParam();
    void tryInitAreaLimitter(const PlacementInfo& placementInfo);
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

    CameraViewInfo* getViewInfo() const { return mViewInfo; }

    // set
    void setPosition(const sead::Vector3f& vec) { mPosition.set(vec); };

    void setTargetTrans(const sead::Vector3f& vec) { mTargetTrans.set(vec); };

    void setCameraUp(const sead::Vector3f& vec) { mCameraUp.set(vec); };

    void setViewMtx(const sead::Matrix34f& mtx) { mViewMtx = mtx; }

    void setFovyDegree(f32 fovy) { mFovyDegree = fovy; }

protected:
    const char* mPoserName;
    ActiveState mActiveState = ActiveState_Start;
    sead::Vector3f mPosition = {0, 0, 0};
    sead::Vector3f mTargetTrans = {0, 500.0f, 0};
    sead::Vector3f mCameraUp = sead::Vector3f::ey;
    f32 mFovyDegree = 35.0f;
    f32 _60 = -1.0f;
    sead::Matrix34f mViewMtx;
    bool _98 = 0;
    CameraPoserControls* mCtrls = nullptr;
    CameraViewInfo* mViewInfo = nullptr;
    CameraPoserFlag* mPoserFlag;
    CameraVerticalAbsorber* mVerticalAbsorber;
    CameraAngleCtrlInfo* mAngleCtrlInfo;
    CameraAngleSwingInfo* mAngleSwingInfo;
    CameraArrowCollider* mArrowCollider;
    CameraOffsetCtrlPreset* mOffsetCtrlPreset;
    LocalInterpole* mLocalInterpole;
    LookAtInterpole* mLookAtInterpole;
    CameraParamMoveLimit* mParamMoveLimit;
    CameraTargetAreaLimitter* mTargetAreaLimitter;
    GyroCameraCtrl* mGyroCtrl;
    SnapShotCameraCtrl* mSnapShotCtrl;
    AudioKeeper* mAudioKeeper;
    NerveKeeper* mNerveKeeper;
    RailKeeper* mRailKeeper;
    CameraInterpolateParam* mActiveInterpoleParam;
    CameraInterpolateStep* mEndInterpoleParam;
    OrthoProjectionParam* mOrthoProjectionParam;
};

static_assert(sizeof(CameraPoser) == 0x140);

}  // namespace al
