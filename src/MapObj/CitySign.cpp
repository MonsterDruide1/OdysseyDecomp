#include "MapObj/CitySign.h"

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
NERVE_IMPL(CitySign, Wait);
NERVE_IMPL(CitySign, LightOff);

NERVES_MAKE_STRUCT(CitySign, Wait, LightOff);
}  // namespace

CitySign::CitySign(const char* name) : al::LiveActor(name) {}

void CitySign::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &NrvCitySign.Wait, 0);

    s32 scenarioNo = GameDataFunction::getScenarioNo(this);
    al::tryStartAction(this, al::StringTmp<64>("Scenario%d", scenarioNo).cstr());

    makeActorAlive();
}

bool CitySign::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (al::isNerve(this, &NrvCitySign.Wait) && al::isMsgExplosion(message)) {
        al::startAction(this, "LightOff");
        al::tryOnStageSwitch(this, "AttackedOn");

        sead::Vector3f offset = sead::Vector3f::zero;
        const char* modelName = al::getModelName(this);
        if (al::isEqualString(modelName, "CityWorldHomeSign002"))
            offset = {0.0f, 850.0f, 0.0f};
        else if (al::isEqualString(modelName, "CityWorldHomeSign003"))
            offset = {0.0f, 900.0f, 0.0f};
        else if (al::isEqualString(modelName, "CityWorldHomeSign005"))
            offset = {0.0f, 750.0f, 0.0f};
        else if (al::isEqualString(modelName, "CityWorldHomeSign004"))
            offset = {0.0f, 750.0f, 0.0f};
        else if (al::isEqualString(modelName, "CityWorldHomeSign006"))
            offset = {0.0f, 800.0f, 0.0f};
        else if (al::isEqualString(modelName, "CityWorldHomeSign016"))
            offset = {-1000.0f, 2500.0f, 500.0f};

        sead::Quatf quat = sead::Quatf::unit;
        al::calcQuat(&quat, this);
        offset.setRotated(quat, offset);

        rs::setAppearItemFactorByMsg(this, message, other);
        al::setAppearItemOffset(this, offset);
        al::appearItem(this);
        al::setNerve(this, &NrvCitySign.LightOff);
    }

    return false;
}

void CitySign::exeWait() {}

void CitySign::exeLightOff() {}
