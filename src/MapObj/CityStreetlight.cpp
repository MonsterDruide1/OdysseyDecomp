#include "MapObj/CityStreetlight.h"

#include "Library/Base/StringUtil.h"
#include "Library/Item/ItemUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Library/Stage/StageSwitchUtil.h"
#include "System/GameDataFunction.h"
#include "Util/ItemUtil.h"

namespace {
NERVE_IMPL(CityStreetlight, WaitOn);
NERVE_IMPL(CityStreetlight, WaitOff);

NERVES_MAKE_STRUCT(CityStreetlight, WaitOn, WaitOff);
}  // namespace

CityStreetlight::CityStreetlight(const char* name) : al::LiveActor(name) {}

void CityStreetlight::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    if (GameDataFunction::getScenarioNo(this) == 1)
        al::initNerve(this, &NrvCityStreetlight.WaitOn, 0);
    else
        al::initNerve(this, &NrvCityStreetlight.WaitOff, 0);
    al::trySyncStageSwitchAppearAndKill(this);
}

bool CityStreetlight::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                 al::HitSensor* self) {
    if (!al::isMsgPlayerDisregard(message)) {
        if (!al::isMsgExplosion(message) || !al::isSensorMapObj(self) ||
            !al::isNerve(this, &NrvCityStreetlight.WaitOn))
            return false;

        al::setNerve(this, &NrvCityStreetlight.WaitOff);
        al::tryOnStageSwitch(this, "AttackedOn");

        sead::Vector3f offset = sead::Vector3f::zero;
        const char* modelName = al::getModelName(this);
        if (al::isEqualString(modelName, "StreetlightA"))
            offset = {0.0f, 700.0f, 0.0f};
        else if (al::isEqualString(modelName, "CityWorldHomeSignal001"))
            offset = {-600.0f, 1400.0f, 0.0f};
        else if (al::isEqualString(modelName, "CityWorldHomeStreetlight000"))
            offset = {0.0f, 750.0f, 250.0f};

        sead::Quatf quat = sead::Quatf::unit;
        al::calcQuat(&quat, this);
        offset.setRotated(quat, offset);

        rs::setAppearItemFactorByMsg(this, message, other);
        al::setAppearItemOffset(this, offset);
        al::appearItem(this);
    }

    return true;
}

void CityStreetlight::calcAnim() {
    if (al::isNerve(this, &NrvCityStreetlight.WaitOff) && al::isGreaterEqualStep(this, 2))
        al::calcViewModel(this);
    else
        LiveActor::calcAnim();
}

void CityStreetlight::exeWaitOn() {
    if (al::isFirstStep(this))
        al::startAction(this, "LightOn");
}

void CityStreetlight::exeWaitOff() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "LightOff");
        al::invalidateHitSensor(this, "Body");
    }
}
