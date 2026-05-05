#include "MapObj/ShopCamera.h"

#include <math/seadVector.h>

#include "Library/Camera/CameraUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
struct ShopCameraParam {
    ShopCameraParam() : cameraPos(sead::Vector3f::zero) {
        cameraAt.x = 0.0f;
        cameraAt.y = 0.01f;
        cameraAt.z = 1.0f;
    }

    sead::Vector3f cameraAt;
    sead::Vector3f cameraPos;
};

ShopCameraParam sCameraParam;

NERVE_IMPL(ShopCamera, None);
NERVE_IMPL(ShopCamera, Appear);
NERVE_IMPL(ShopCamera, Wait);
NERVE_IMPL(ShopCamera, End);

NERVES_MAKE_NOSTRUCT(ShopCamera, None, Appear, Wait, End);
}  // namespace

ShopCamera::ShopCamera(const al::LiveActor* actor)
    : mActor(actor), mCameraPos(sCameraParam.cameraPos), mCameraAt(sCameraParam.cameraAt) {
    mNerveKeeper = new al::NerveKeeper(this, &None, 0);
    mCameraTicket = al::initProgramableCamera(mActor, "ショップ内のライト方向統一用カメラ",
                                              &mCameraPos, &mCameraAt, nullptr);
}

void ShopCamera::execute() {
    getNerveKeeper()->update();
}

void ShopCamera::tryStart() {
    if (al::isNerve(this, &None)) {
        mCameraPos.set(sCameraParam.cameraPos);
        mCameraAt.set(sCameraParam.cameraAt);
        mStartCameraPos = al::getCameraPos(mActor, 0);
        mStartCameraAt = al::getCameraAt(mActor, 0);
        al::setNerve(this, &Appear);
    }
}

void ShopCamera::tryEnd() {
    if (al::isNerve(this, &Wait)) {
        mCameraPos.set(mStartCameraPos);
        mCameraAt.set(mStartCameraAt);
        al::setNerve(this, &End);
    }
}

void ShopCamera::exeAppear() {
    if (al::isFirstStep(this))
        al::startCamera(mActor, mCameraTicket, 0);

    al::setNerve(this, &Wait);
}

void ShopCamera::exeWait() {}

void ShopCamera::exeEnd() {
    if (al::isStep(this, 1)) {
        al::endCamera(mActor, mCameraTicket, 0, false);
        al::setNerve(this, &None);
    }
}

void ShopCamera::exeNone() {}
