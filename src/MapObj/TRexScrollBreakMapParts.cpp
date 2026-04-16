#include "MapObj/TRexScrollBreakMapParts.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Scene/SceneObjUtil.h"

#include "MapObj/TRexScrollBreakMapPartsBreakJudge.h"
#include "Util/SensorMsgFunction.h"

namespace {
class TRexScrollBreakMapPartsNrvWait : public al::Nerve {
public:
    void execute(al::NerveKeeper* keeper) const override {
        keeper->getParent<TRexScrollBreakMapParts>()->exeWait();
    }
};

class TRexScrollBreakMapPartsNrvBreak : public al::Nerve {
public:
    void execute(al::NerveKeeper* keeper) const override {
        keeper->getParent<TRexScrollBreakMapParts>()->kill();
    }
};

TRexScrollBreakMapPartsNrvWait Wait;
TRexScrollBreakMapPartsNrvBreak Break;
}  // namespace

TRexScrollBreakMapParts::TRexScrollBreakMapParts(const char* actorName)
    : al::LiveActor(actorName) {}

void TRexScrollBreakMapParts::init(const al::ActorInitInfo& info) {
    al::initMapPartsActor(this, info, nullptr);
    al::initNerve(this, &Wait, 0);

    if (al::isExistCollisionParts(this))
        mBreakSensors.allocBuffer(64, nullptr, 8);

    makeActorAlive();
}

bool TRexScrollBreakMapParts::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                         al::HitSensor* self) {
    if (!rs::isMsgConfirmFrailBox(message))
        return false;

    if (mBreakSensors.size() < mBreakSensors.capacity())
        mBreakSensors.pushBack(other);

    return true;
}

void TRexScrollBreakMapParts::exeWait() {
    if (!al::isExistSceneObj<TRexScrollBreakMapPartsBreakJudge>(this))
        return;

    TRexScrollBreakMapPartsBreakJudge* breakJudge =
        al::getSceneObj<TRexScrollBreakMapPartsBreakJudge>(this);
    if (!breakJudge->judgeBreak(this))
        return;

    al::startHitReaction(this, "破壊");
    breakJudge->addBreakCountAndAppearBreakModel(this);

    if (al::isExistCollisionParts(this)) {
        s32 breakSensorCount = mBreakSensors.size();
        al::HitSensor* collisionSensor = al::getHitSensor(this, "Collision");

        if (breakSensorCount >= 1) {
            for (s32 sensorIndex = 0; sensorIndex != breakSensorCount; sensorIndex++) {
                al::HitSensor* breakSensor = nullptr;

                if (u32(mBreakSensors.size()) > u32(sensorIndex))
                    breakSensor = mBreakSensors.unsafeAt(sensorIndex);

                rs::sendMsgTRexScrollPartsBreakWith(breakSensor, collisionSensor);
            }
        }
    }

    al::setNerve(this, &Break);
}
