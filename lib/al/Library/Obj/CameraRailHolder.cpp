#include "Library/Obj/CameraRailHolder.h"

#include "Library/Camera/CameraDirector.h"
#include "Library/Camera/CameraLimitRailKeeper.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/LiveActor/ActorDrawFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"

namespace al {
CameraRailHolder::CameraRailHolder(const char* name) : LiveActor(name) {}

void CameraRailHolder::init(const ActorInitInfo& info) {
    initActorSceneInfo(this, info);
    initExecutorWatchObj(this, info);
    info.getActorSceneInfo().mCameraDirector->registerCameraRailHolder(this);

    mCameraRailCount = calcLinkChildNum(info, "Rail");

    if (mCameraRailCount > 0) {
        mCameraRails = new CameraLimitRailKeeper*[mCameraRailCount];

        for (s32 i = 0; i < mCameraRailCount; i++) {
            PlacementInfo placementInfo;

            getLinksInfoByIndex(&placementInfo, info, "Rail", i);
            mCameraRails[i] = new CameraLimitRailKeeper();

            mCameraRails[i]->init(placementInfo, getViewNumMax(this));
        }
    }

    makeActorDead();
}
}  // namespace al
