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

    class AreaObj : public al::IUseStageSwitch, public al::IUseSceneObjHolder, public al::HioNode {
    public:
        AreaObj(const char* name);

        virtual const char* getName() const override;
        virtual StageSwitchKeeper* getStageSwitchKeeper() const override;
        virtual void initStageSwitchKeeper() override;
        virtual void init(const AreaInitInfo& info);
        virtual bool isInVolume(const sead::Vector3f& pos) const;
        virtual bool isInVolumeOffset(const sead::Vector3f& pos, f32 offset) const;
        virtual SceneObjHolder* getSceneObjHolder() const override;

        const char *mName; // 0x10
        AreaShape *mAreaShape; // 0x18
        StageSwitchKeeper *mStageSwitchKeeper; // 0x20
        SceneObjHolder *mSceneObjHolder; // 0x28
        sead::Matrix34f mAreaTR; // 0x30
        PlacementInfo *mPlacementInfo; // 0x60
        s32 mPriority; // 0x68
        bool mIsValid; // 0x6C

    };
}
