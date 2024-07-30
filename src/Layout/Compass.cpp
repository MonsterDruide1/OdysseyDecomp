#include "Layout/Compass.h"
#include "System/GameDataHolderAccessor.h"
#include "System/GameDataFunction.h"

#include "Library/Area/AreaObjUtil.h"
#include "Library/Camera/CameraDirector.h"
#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Player/PlayerUtil.h"

namespace {
NERVE_IMPL(Compass, Appear);
NERVE_IMPL(Compass, End);
NERVE_IMPL(Compass, Wait);

NERVES_MAKE_NOSTRUCT(Compass, Appear, End, Wait);
}

Compass::Compass(const char* name, const al::LayoutInitInfo& info, const al::PlayerHolder* playerHolder) :
    al::LayoutActor(name),
    mSceneCamInfo(nullptr),
    mPlayerHolder(playerHolder),
    m140(0.0f, 0.0f, 0.0f),
    m14C(0.0f) {
    al::initLayoutActor(this, info, "TestCompass", nullptr);
    initNerve(&Appear, 0);

    mSceneCamInfo = getCameraDirector()->getSceneCameraInfo();
    kill();
}

/*void Compass::appear() {
    GameDataHolderAccessor accessor(this);

    if (GameDataFunction::isMainStage(accessor)) {
        al::LayoutActor::appear();
        al::setNerve(this, &Appear);
        m14C = 0.0f;

        al::LiveActor* player = al::tryGetPlayerActor(mPlayerHolder, 0);

        if (player != nullptr) {
            al::AreaObj* area = al::tryFindAreaObj(player, "CompassArea", al::getTrans(player);

            if (area != nullptr) {
                bool isMadness;

                if (al::tryGetAreaObjArg(&isMadness, area, "IsMadness") && isMadness) {
                    return;
                }
            }
        }

        sead::Vector3f camDir { 0.0f, 0.0f, 0.0f };

        if (al::tryCalcCameraLookDirH(&camDir, mSceneCamInfo, sead::Vector3f::ey, 0)) {
            sead::Vector3f northDir { 0.0f, 0.0f, 0.0f };

            if (rs::tryCalcMapNorthDir(&northDir, this)) {
                // ...
            }
        }
    }
    else {
        al::LiveActor* player = al::tryGetPlayerActor(mPlayerHolder, 0);

        if (player != nullptr && al::isInAreaObj(player, "CompassArea", al::getTrans(player))) {
            al::LayoutActor::appear();
            al::setNerve(this, &Appear);
            m14C = 0.0f;
        }
    }
}*/

void Compass::end() {
    if (!al::isNerve(this, &End)) {
        al::setNerve(this, &End);
    }
}

void Compass::exeAppear() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Appear", nullptr);
    }

    updateLayout();
    al::setNerveAtActionEnd(this, &Wait);
}

// void Compass::updateLayout();

void Compass::exeWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait", nullptr);
    }

    updateLayout();
}

void Compass::exeEnd() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "End", nullptr);
    }

    updateLayout();

    if (al::isActionEnd(this, nullptr)) {
        kill();
    }
}
