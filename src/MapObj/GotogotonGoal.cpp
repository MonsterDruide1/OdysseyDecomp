#include "MapObj/GotogotonGoal.h"

#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"

#include "Enemy/GotogotonMark.h"
#include "Util/SensorMsgFunction.h"

GotogotonGoal::GotogotonGoal(const char* name) : al::LiveActor(name) {}

void GotogotonGoal::init(const al::ActorInitInfo& info) {
    al::initMapPartsActor(this, info, nullptr);
    mMark = new GotogotonMark();
    al::calcFrontDir(&mMark->mFrontDir, this);
    if (al::isExistModelResourceYaml(this, "GotogotonMarkInfo", nullptr)) {
        const u8* modelResourceYaml = al::getModelResourceYaml(this, "GotogotonMarkInfo", nullptr);
        al::ByamlIter iter(modelResourceYaml);
        al::tryGetByamlS32(&mMark->mId, iter, "MarkId");
    }

    makeActorAlive();
}

bool GotogotonGoal::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                               al::HitSensor* self) {
    if (rs::isMsgGotogotonGoalExist(message))
        return true;

    return rs::isMatchGotogotonGoal(message, mMark);
}
