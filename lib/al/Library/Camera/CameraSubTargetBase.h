#pragma once

#include "Library/Area/IUseAreaObj.h"
#include "Library/Camera/CameraTargetBase.h"

namespace al {
class Scene;
class CameraSubTargetTurnParam;

class CameraSubTargetBase : public CameraTargetBase {
public:
    CameraSubTargetBase();

    virtual void calcSide(sead::Vector3f* side) const override;
    virtual void calcUp(sead::Vector3f* up) const override;
    virtual void calcFront(sead::Vector3f* front) const override;
    virtual void calcVelocity(sead::Vector3f* velocity) const override;

private:
    CameraSubTargetTurnParam* mTurnParam;
};

class CameraStopJudge : public al::HioNode, al::IUseAreaObj {
public:
    void init();
    CameraStopJudge();
    bool isStop() const;
    void update(sead::Vector3f const&);
    al::AreaObjDirector* getAreaObjDirector() const override;

private:
    bool mIsInObjArea;
    bool mIsNotInObjArea;
    bool mStopJudge;
    AreaObjDirector* mAreaObjDirector;
};

static_assert(sizeof(CameraSubTargetBase) == 0x18);
}  // namespace al
