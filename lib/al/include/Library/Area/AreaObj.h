#pragma once

#include <math/seadMatrix.h>

#include "Library/HostIO/HioNode.h"
#include "Library/Scene/IUseSceneObjHolder.h"
#include "Library/Stage/IUseStageSwitch.h"

namespace al {
class AreaInitInfo;
class AreaObjGroup;
class AreaShape;
class PlacementInfo;
class SceneObjHolder;
class StageSwitchKeeper;

class AreaObj : public IUseStageSwitch, public IUseSceneObjHolder, public HioNode {
public:
    AreaObj(const char* name);
    const char* getName() const override;
    StageSwitchKeeper* getStageSwitchKeeper() const override;
    void initStageSwitchKeeper() override;
    virtual void init(const AreaInitInfo& initInfo);
    virtual bool isInVolume(const sead::Vector3f& position) const;
    virtual bool isInVolumeOffset(const sead::Vector3f& position, f32 offset) const;
    SceneObjHolder* getSceneObjHolder() const override;
    void validate();
    void invalidate();

    s32 getPriority() { return mPriority; };

private:
    const char* mName;
    AreaShape* mAreaShape = nullptr;
    StageSwitchKeeper* mStageSwitchKeeper = nullptr;
    SceneObjHolder* mSceneObjHolder = nullptr;
    sead::Matrix34f mMatrixTR = sead::Matrix34f::ident;
    PlacementInfo* mPlacementInfo = nullptr;
    s32 mPriority = -1;
    bool isValid = true;
};

}  // namespace al
