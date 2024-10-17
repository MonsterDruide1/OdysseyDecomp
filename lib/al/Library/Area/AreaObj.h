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

    virtual const char* getName() const override;
    StageSwitchKeeper* getStageSwitchKeeper() const override;
    void initStageSwitchKeeper() override;
    void init(const AreaInitInfo& info);
    bool isInVolume(const sead::Vector3f& pos) const;
    bool isInVolumeOffset(const sead::Vector3f& pos, f32 offset) const;
    SceneObjHolder* getSceneObjHolder() const override;

private:
    const char* mName;
    AreaShape* mAreaShape;
    StageSwitchKeeper* mStageSwitchKeeper;
    SceneObjHolder* mSceneObjHolder;
    sead::Matrix34f mAreaTR;
    PlacementInfo* mPlacementInfo;
    s32 mPriority;
    bool mIsValid;
};
}  // namespace al
