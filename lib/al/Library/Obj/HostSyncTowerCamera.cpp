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
NERVE_IMPL_(HostSyncTowerCamera, Deactive2, Deactive);
NERVE_IMPL_(HostSyncTowerCamera, Active2, Active);
NERVES_MAKE_NOSTRUCT(HostSyncTowerCamera, Deactive, Active, Deactive2, Active2);
}  // namespace

namespace al {
HostSyncTowerCamera::HostSyncTowerCamera(LiveActor* actor, AreaObjGroup* activationArea,
                                         const ActorInitInfo& info)
    : NerveExecutor("ホスト同期タワーカメラ制御"), mActor(actor), mActivationArea(activationArea) {
    initNerve(&Deactive);
    mTowerCamera =
        initTowerCameraWithSave(mActor, getTransPtr(mActor), info, "アクター追従塔カメラ");
}

void HostSyncTowerCamera::update() {
    updateNerve();
}

void HostSyncTowerCamera::activate() {
    if (!isNerve(this, &Active) && mTowerCamera && mActivationArea)
        setNerve(this, &Active);
}

void HostSyncTowerCamera::deactivate() {
    if (!isNerve(this, &Deactive)) {
        tryEndCamera();
        setNerve(this, &Deactive);
    }
}

void HostSyncTowerCamera::tryEndCamera() {
    if (mTowerCamera && isActiveCamera(mTowerCamera))
        endCamera(mActor, mTowerCamera, -1, false);
}

void HostSyncTowerCamera::exeActive() {
    AreaObjGroup* areaObjGroup = mActivationArea;
    LiveActor* playerActor = tryFindNearestPlayerActor(mActor);
    if (playerActor && isInAreaObj(areaObjGroup, getTrans(playerActor)))
        tryStartCamera();
    else
        tryEndCamera();
}

void HostSyncTowerCamera::tryStartCamera() {
    if (mTowerCamera && !isActiveCamera(mTowerCamera))
        startCamera(mActor, mTowerCamera);
}

void HostSyncTowerCamera::exeDeactive() {}

}  // namespace al
