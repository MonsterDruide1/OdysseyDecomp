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

    void initNerve(const Nerve*, int);
    void initArrowCollider(CameraArrowCollider*);
    void initAudioKeeper(const char*);
    void initRail(const PlacementInfo&);
    void initLocalInterpole();
    void initLookAtInterpole(float);
    void initOrthoProjectionParam();
    void tryInitAreaLimitter(const PlacementInfo&);

    void makeLookAtCameraPrev(sead::LookAtCamera*) const;
    void makeLookAtCameraPost(sead::LookAtCamera*) const;
    void makeLookAtCameraLast(sead::LookAtCamera*) const;
    void makeLookAtCameraCollide(sead::LookAtCamera*) const;

    void getInterpoleStep();
    void setInterpoleStep(int);
    void resetInterpoleStep();
    void setInterpoleEaseOut();
    void getEndInterpoleStep();

    void appear(const CameraStartInfo&);
    void calcCameraPose(sead::LookAtCamera*);
    void receiveRequestFromObjectCore(const CameraObjectRequestInfo&);

    void startSnapShotModeCore();
    void endSnapShotModeCore();

    float getFovyDegree() const;

    sead::Vector3f getPosition() const { return mPosition; };
    sead::Vector3f getTargetTrans() const { return mTargetTrans; };
    sead::Vector3f getCameraUp() const { return mCameraUp; };

private:
    const char* mPoserName;
    float field_38;
    sead::Vector3f mPosition;
    sead::Vector3f mTargetTrans = sead::Vector3f::ex;
    sead::Vector3f mCameraUp = sead::Vector3f::ey;
    float mFovyDegree = 35.0f;
    float field_64;
    sead::Matrix34f mViewMtx = sead::Matrix34f::ident;
    bool field_98 = false;
    CameraViewInfo* mViewInfo;
    AreaObjDirector* mAreaDirector;
    CameraPoserFlag* mPoserFlags;
    CameraVerticalAbsorber* mVerticalAbsorber;
    CameraAngleCtrlInfo* mAngleCtrlInfo;
    CameraAngleSwingInfo* mAngleSwingInfo;
    CameraArrowCollider* mArrowCollider;
    CameraOffsetCtrlPreset* mOffsetCtrlPreset;
    float* mLocalInterpole;   // size = 0x20
    float* mLookAtInterpole;  // size = 0x10
    CameraParamMoveLimit* mParamMoveLimit;
    void* field_f8;
    GyroCameraCtrl* mGyroCtrl;
    SnapShotCameraCtrl* mSnapshotCtrl;
    AudioKeeper* mAudioKeeper;
    NerveKeeper* mNerveKeeper;
    RailKeeper* mRailKeeper;
    int* field_128;  // (size = 0xC) interpolesteptype?
    int* field_130;  // (size - 0x8)
    sead::Vector3f* mOrthoProjectionParam;
};
}  // namespace al
