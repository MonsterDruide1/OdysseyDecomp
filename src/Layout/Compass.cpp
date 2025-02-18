#include "Layout/Compass.h"

#include "Library/Area/AreaObjUtil.h"
#include "Library/Camera/CameraDirector.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"

#include "Layout/MapLayout.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"

namespace {
NERVE_IMPL(Compass, Appear);
NERVE_IMPL(Compass, End);
NERVE_IMPL(Compass, Wait);

NERVES_MAKE_NOSTRUCT(Compass, Appear, End, Wait);
}  // namespace

namespace {
bool isAreaMadness(al::AreaObj* area) {
    bool isMadness = false;
    return al::tryGetAreaObjArg(&isMadness, area, "IsMadness") && isMadness;
}
}  // namespace

Compass::Compass(const char* name, const al::LayoutInitInfo& info,
                 const al::PlayerHolder* playerHolder)
    : al::LayoutActor(name), mPlayerHolder(playerHolder) {
    al::initLayoutActor(this, info, "TestCompass", nullptr);
    initNerve(&Appear, 0);

    mSceneCamInfo = getCameraDirector()->getSceneCameraInfo();
    kill();
}

void Compass::appear() {
    if (GameDataFunction::isMainStage(this)) {
        al::LayoutActor::appear();
        al::setNerve(this, &Appear);
        field_14c = 0.0f;

        al::LiveActor* player = al::tryGetPlayerActor(mPlayerHolder, 0);

        if (player != nullptr) {
            al::AreaObj* area = al::tryFindAreaObj(player, "CompassArea", al::getTrans(player));

            if (area != nullptr && isAreaMadness(area))
                return;
        }

        sead::Vector3f camDir{0.0f, 0.0f, 0.0f};

        if (!al::tryCalcCameraLookDirH(&camDir, mSceneCamInfo, sead::Vector3f::ey, 0))
            return;

        sead::Vector3f northDir{0.0f, 0.0f, 0.0f};

        if (!rs::tryCalcMapNorthDir(&northDir, this))
            return;

        f32 angle = al::calcAngleOnPlaneDegree(northDir, camDir, -sead::Vector3f::ey);
        angle = al::modf(angle + 360.0f, 360.0f) + 0.0f;

        f32 maxFrame = al::getActionFrameMax(this, "Direction", "State");
        f32 frame = al::normalize(angle, 0.0f, maxFrame);
        al::startFreezeAction(this, "Direction", frame, "State");
    } else {
        al::LiveActor* player = al::tryGetPlayerActor(mPlayerHolder, 0);

        if (player == nullptr || !al::isInAreaObj(player, "CompassArea", al::getTrans(player)))
            return;

        al::LayoutActor::appear();
        al::setNerve(this, &Appear);
        field_14c = 0.0f;
    }
}

void Compass::end() {
    if (!al::isNerve(this, &End))
        al::setNerve(this, &End);
}

void Compass::exeAppear() {
    if (al::isFirstStep(this))
        al::startAction(this, "Appear", nullptr);

    updateLayout();
    al::setNerveAtActionEnd(this, &Wait);
}

// void Compass::updateLayout();

void Compass::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait", nullptr);

    updateLayout();
}

void Compass::exeEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "End", nullptr);

    updateLayout();

    if (al::isActionEnd(this, nullptr))
        kill();
}
