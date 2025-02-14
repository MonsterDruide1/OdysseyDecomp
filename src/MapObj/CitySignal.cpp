#include "MapObj/CitySignal.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "System/GameDataFunction.h"

namespace {
NERVE_IMPL(CitySignal, WaitRed);
NERVE_IMPL(CitySignal, WaitBlue);
NERVE_IMPL(CitySignal, WaitOff);

NERVES_MAKE_STRUCT(CitySignal, WaitRed, WaitBlue, WaitOff);
}  // namespace

CitySignal::CitySignal(const char* name) : al::LiveActor(name) {}

void CitySignal::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);

    s32 scenarioNo = GameDataFunction::getScenarioNo(this);
    al::tryStartAction(this, al::StringTmp<64>("Scenario%d", scenarioNo).cstr());

    if (scenarioNo == 1)
        al::initNerve(this, &NrvCitySignal.WaitRed, 0);
    else
        al::initNerve(this, &NrvCitySignal.WaitBlue, 0);

    al::trySyncStageSwitchAppearAndKill(this);
}

void CitySignal::movement() {
    // this being empty results in the animation not playing (blinking for Red)
    // and the signal not turning off when hit by an explosion
}

void CitySignal::calcAnim() {
    if (al::isNerve(this, &NrvCitySignal.WaitRed) || al::isNerve(this, &NrvCitySignal.WaitOff))
        al::LiveActor::calcAnim();
    else
        al::calcViewModel(this);
}

bool CitySignal::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                            al::HitSensor* self) {
    if (al::isMsgExplosion(message) && al::isSensorMapObj(self) &&
        (al::isNerve(this, &NrvCitySignal.WaitRed) || al::isNerve(this, &NrvCitySignal.WaitBlue))) {
        al::setNerve(this, &NrvCitySignal.WaitOff);
        return true;
    }

    return false;
}

void CitySignal::exeWaitRed() {}

void CitySignal::exeWaitBlue() {}

void CitySignal::exeWaitOff() {
    if (al::isFirstStep(this))
        al::startAction(this, "LightOff");
}
