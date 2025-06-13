#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/HostIO/HioNode.h"
#include "Library/Scene/IUseSceneObjHolder.h"
#include "Library/Stage/IUseStageSwitch.h"

namespace al {

class AreaShape;
class PlacementInfo;
class AreaInitInfo;

class AreaObj : public IUseStageSwitch, public IUseSceneObjHolder, public HioNode {
public:
    AreaObj(const char* name);

    const char* getName() const override { return mName; }

    StageSwitchKeeper* getStageSwitchKeeper() const override { return mStageSwitchKeeper; }

    void initStageSwitchKeeper() override;
    virtual void init(const AreaInitInfo& info);
    virtual bool isInVolume(const sead::Vector3f& pos) const;
    virtual bool isInVolumeOffset(const sead::Vector3f& pos, f32 offset) const;

    SceneObjHolder* getSceneObjHolder() const override { return mSceneObjHolder; }

    void validate() { mIsValid = true; }

    void invalidate() { mIsValid = false; }

    const PlacementInfo* getPlacementInfo() const { return mPlacementInfo; }

    AreaShape* getAreaShape() const { return mAreaShape; }

    const sead::Matrix34f& getAreaMtx() const { return mAreaTR; }

    s32 getPriority() const { return mPriority; }

    bool isValid() const { return mIsValid; }

private:
    const char* mName;
    AreaShape* mAreaShape = nullptr;
    StageSwitchKeeper* mStageSwitchKeeper = nullptr;
    SceneObjHolder* mSceneObjHolder = nullptr;
    sead::Matrix34f mAreaTR = sead::Matrix34f::ident;
    PlacementInfo* mPlacementInfo = nullptr;
    s32 mPriority = -1;
    bool mIsValid = true;
};
}  // namespace al
