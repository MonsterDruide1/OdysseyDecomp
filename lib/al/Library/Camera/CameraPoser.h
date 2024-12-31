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
#include "Library/Rail/IUseRail.h"

namespace al {
class CameraVerticalAbsorber;
class CameraAngleCtrlInfo;
class CameraAngleSwingInfo;
class CameraArrowCollider;
class CameraOffsetCtrlPreset;
class CameraParamMoveLimit;
class GyroCameraCtrl;
class CameraViewInfo;
class CameraStartInfo;
class CameraObjectRequestInfo;
class CameraTurnInfo;
class CameraPoserFlag;
class RailKeeper;
class PlacementInfo;
class ByamlIter;
class Nerve;
class SnapShotCameraCtrl;

class CameraPoser : public HioNode,
                    public IUseAreaObj,
                    public IUseAudioKeeper,
                    public IUseCollision,
                    public IUseName,
                    public IUseNerve,
                    public IUseRail {
public:
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
    virtual void receiveRequestFromObject(const CameraObjectRequestInfo&);
    virtual bool isZooming() const;
    virtual bool isEnableRotateByPad() const;
    virtual void startSnapShotMode();
    virtual void endSnapShotMode();

    virtual const char* getName() const override;
    virtual CollisionDirector* getCollisionDirector() const override;
    virtual NerveKeeper* getNerveKeeper() const override;
    virtual AudioKeeper* getAudioKeeper() const override;
    virtual RailRider* getRailRider() const override;

    virtual void load(const ByamlIter&);
    virtual void movement();
    virtual void calcCameraPose(sead::LookAtCamera*) const;
    virtual void requestTurnToDirection(const CameraTurnInfo*);

    bool isInterpoleByCameraDistance() const;
    bool isInterpoleEaseOut() const;
    bool isEndInterpoleByStep() const;
    bool isFirstCalc() const;

    void initNerve(const Nerve*, s32);
    void initArrowCollider(CameraArrowCollider*);
    void initAudioKeeper(const char*);
    void initRail(const PlacementInfo&);
    void initLocalInterpole();
    void initLookAtInterpole(f32);
    void initOrthoProjectionParam();
    void tryInitAreaLimitter(const PlacementInfo&);

    void makeLookAtCameraPrev(sead::LookAtCamera*) const;
    void makeLookAtCameraPost(sead::LookAtCamera*) const;
    void makeLookAtCameraLast(sead::LookAtCamera*) const;
    void makeLookAtCameraCollide(sead::LookAtCamera*) const;

    s32 getInterpoleStep();
    void setInterpoleStep(s32);
    void resetInterpoleStep();
    void setInterpoleEaseOut();
    s32 getEndInterpoleStep();

    void appear(const CameraStartInfo&);
    void receiveRequestFromObjectCore(const CameraObjectRequestInfo&);

    void startSnapShotModeCore();
    void endSnapShotModeCore();

    f32 getFovyDegree() const;

    const sead::Vector3f& getPosition() const { return mPosition; };

    const sead::Vector3f& getTargetTrans() const { return mTargetTrans; };

    const sead::Vector3f& getCameraUp() const { return mCameraUp; };

    const sead::Matrix34f& getViewMtx() const { return mViewMtx; };

    CameraPoserFlag* getPoserFlag() const { return mPoserFlag; }

    void setField98(bool flag) { field_98 = flag; }

private:
    const char* mPoserName;
    f32 field_38;
    sead::Vector3f mPosition;
    sead::Vector3f mTargetTrans = sead::Vector3f::ex;
    sead::Vector3f mCameraUp = sead::Vector3f::ey;
    f32 mFovyDegree = 35.0f;
    f32 field_64;
    sead::Matrix34f mViewMtx = sead::Matrix34f::ident;
    bool field_98 = false;
    CameraViewInfo* mViewInfo;
    AreaObjDirector* mAreaDirector;
    CameraPoserFlag* mPoserFlag;
    CameraVerticalAbsorber* mVerticalAbsorber;
    CameraAngleCtrlInfo* mAngleCtrlInfo;
    CameraAngleSwingInfo* mAngleSwingInfo;
    CameraArrowCollider* mArrowCollider;
    CameraOffsetCtrlPreset* mOffsetCtrlPreset;
    f32* mLocalInterpole;   // size = 0x20
    f32* mLookAtInterpole;  // size = 0x10
    CameraParamMoveLimit* mParamMoveLimit;
    void* field_f8;
    GyroCameraCtrl* mGyroCtrl;
    SnapShotCameraCtrl* mSnapshotCtrl;
    AudioKeeper* mAudioKeeper;
    NerveKeeper* mNerveKeeper;
    RailKeeper* mRailKeeper;
    s32* field_128;  // (size = 0xC) interpolesteptype?
    s32* field_130;  // (size - 0x8)
    sead::Vector3f* mOrthoProjectionParam;
};
}  // namespace al
