#include "Library/Obj/HostSyncTowerCamera.h"

#include "Library/Area/AreaObjUtil.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"

namespace {
using namespace al;
NERVE_IMPL(HostSyncTowerCamera, Deactive);
NERVE_IMPL(HostSyncTowerCamera, Active);
NERVES_MAKE_NOSTRUCT(HostSyncTowerCamera, Deactive, Active);
}  // namespace

namespace al {
HostSyncTowerCamera::HostSyncTowerCamera(LiveActor* actor, AreaObjGroup* areaObjGroup,
                                         const ActorInitInfo& info)
    : NerveExecutor("ホスト同期タワーカメラ制御"), mActor(actor), mAreaObjGroup(areaObjGroup) {
    initNerve(&Deactive);
    mCamera = initTowerCameraWithSave(mActor, getTransPtr(mActor), info, "アクター追従塔カメラ");
}

void HostSyncTowerCamera::update() {
    updateNerve();
}

void HostSyncTowerCamera::activate() {
    if (!isNerve(this, &Active) && mCamera && mAreaObjGroup)
        setNerve(this, &Active);
}

void HostSyncTowerCamera::deactivate() {
    if (!isNerve(this, &Deactive)) {
        tryEndCamera();
        setNerve(this, &Deactive);
    }
}

void HostSyncTowerCamera::tryEndCamera() {
    if (mCamera && isActiveCamera(mCamera))
        endCamera(mActor, mCamera, -1, false);
}

void HostSyncTowerCamera::exeActive() {
    AreaObjGroup* areaObjGroup = mAreaObjGroup;
    LiveActor* playerActor = tryFindNearestPlayerActor(mActor);
    if (playerActor && isInAreaObj(areaObjGroup, getTrans(playerActor))) {
        tryStartCamera();
        return;
    }
    tryEndCamera();
}

void HostSyncTowerCamera::tryStartCamera() {
    if (mCamera && !isActiveCamera(mCamera))
        startCamera(mActor, mCamera, -1);
}

void HostSyncTowerCamera::exeDeactive() {}

}  // namespace al
