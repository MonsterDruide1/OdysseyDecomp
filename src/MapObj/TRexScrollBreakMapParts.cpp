#include "MapObj/TRexScrollBreakMapParts.h"

#include <cmath>

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/BreakModel.h"
#include "Library/Obj/PartsFunction.h"
#include "Library/Scene/SceneObjUtil.h"

#include "Enemy/TRex.h"
#include "MapObj/TRexScrollBreakMapPartsBreakJudge.h"
#include "Util/SensorMsgFunction.h"

namespace {
// breakCount % 9 -> A, B, C, B, C, A, B, A, C
constexpr u64 sBreakModelCycleMaskA = 0xA1uLL;
constexpr u64 sBreakModelCycleMaskB = 0x4AuLL;

NERVE_IMPL(TRexScrollBreakMapParts, Wait)
NERVE_IMPL(TRexScrollBreakMapParts, Break)

NERVES_MAKE_NOSTRUCT(TRexScrollBreakMapParts, Wait, Break)
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

        for (s32 sensorIndex = 0; sensorIndex < breakSensorCount; sensorIndex++) {
            al::HitSensor* breakSensor = nullptr;

            if (u32(mBreakSensors.size()) > u32(sensorIndex))
                breakSensor = mBreakSensors.unsafeAt(sensorIndex);

            rs::sendMsgTRexScrollPartsBreakWith(breakSensor, collisionSensor);
        }
    }

    al::setNerve(this, &Break);
}

void TRexScrollBreakMapParts::exeBreak() {
    kill();
}

bool TRexScrollBreakMapPartsBreakJudge::judgeBreak(const TRexScrollBreakMapParts* mapParts) const {
    if (!mTRex->isEnableBreakPartsForceScroll())
        return false;

    if (mTRex->isForceScrollEndFallStart())
        return true;

    const sead::Vector3f& tRexTrans = al::getTrans(mTRex);
    const sead::Vector3f& mapPartsTrans = al::getTrans(mapParts);
    sead::Vector3f distance;
    sead::Vector3f front;
    f32 tRexX = tRexTrans.x;
    f32 tRexY = tRexTrans.y;
    f32 tRexZ = tRexTrans.z;
    f32 mapPartsX = mapPartsTrans.x;
    f32 mapPartsY = mapPartsTrans.y;
    f32 mapPartsZ = mapPartsTrans.z;

    front.set(0.0f, 0.0f, 0.0f);
    distance.x = tRexX - mapPartsX;
    distance.y = tRexY - mapPartsY;
    distance.z = tRexZ - mapPartsZ;

    al::calcFrontDir(&front, mapParts);
    al::parallelizeVec(&distance, front, distance);

    if (al::isNearZero(distance, 0.001f))
        return false;

    if (front.dot(distance) < 0.0f)
        return false;

    return sqrtf(distance.x * distance.x + distance.y * distance.y + distance.z * distance.z) >
           500.0f;
}

void TRexScrollBreakMapPartsBreakJudge::addBreakCountAndAppearBreakModel(
    const TRexScrollBreakMapParts* mapParts) {
    s32 breakCount = mBreakCount;
    mBreakCount = breakCount + 1;

    al::LiveActor* breakModel;
    s64 breakModelIndex;
    s64 breakModelCycleIndex = al::modi(breakCount + 9, 9);

    if (((sBreakModelCycleMaskA >> breakModelCycleIndex) & 1) != 0) {
        al::LiveActorGroup* breakModelGroup = mBreakModelGroupA;
        s64 breakModelCount = breakModelGroup->getActorCount();

        if (breakModelCount < 1) {
            breakModel = nullptr;
            breakModelIndex = -1;
        } else {
            breakModelIndex = 0;
            while (true) {
                if (al::isDead(breakModelGroup->getActor(breakModelIndex))) {
                    breakModel = breakModelGroup->getActor(breakModelIndex);
                    break;
                }

                if (++breakModelIndex >= breakModelCount) {
                    breakModel = nullptr;
                    breakModelIndex = -1;
                    break;
                }
            }
        }

        mBreakModelMatricesA[breakModelIndex] = *mapParts->getBaseMtx();
        return breakModel->appear();
    } else if (((sBreakModelCycleMaskB >> breakModelCycleIndex) & 1) == 0) {
        al::LiveActorGroup* breakModelGroup = mBreakModelGroupC;
        s64 breakModelCount = breakModelGroup->getActorCount();

        if (breakModelCount < 1) {
            breakModel = nullptr;
            breakModelIndex = -1;
        } else {
            breakModelIndex = 0;
            while (true) {
                if (al::isDead(breakModelGroup->getActor(breakModelIndex))) {
                    breakModel = breakModelGroup->getActor(breakModelIndex);
                    break;
                }

                if (++breakModelIndex >= breakModelCount) {
                    breakModel = nullptr;
                    breakModelIndex = -1;
                    break;
                }
            }
        }

        mBreakModelMatricesC[breakModelIndex] = *mapParts->getBaseMtx();
        return breakModel->appear();
    } else {
        al::LiveActorGroup* breakModelGroup = mBreakModelGroupB;
        s64 breakModelCount = breakModelGroup->getActorCount();

        if (breakModelCount < 1) {
            breakModel = nullptr;
            breakModelIndex = -1;
        } else {
            breakModelIndex = 0;
            while (true) {
                if (al::isDead(breakModelGroup->getActor(breakModelIndex))) {
                    breakModel = breakModelGroup->getActor(breakModelIndex);
                    break;
                }

                if (++breakModelIndex >= breakModelCount) {
                    breakModel = nullptr;
                    breakModelIndex = -1;
                    break;
                }
            }
        }

        mBreakModelMatricesB[breakModelIndex] = *mapParts->getBaseMtx();
        return breakModel->appear();
    }
}

TRexScrollBreakMapPartsBreakJudge::TRexScrollBreakMapPartsBreakJudge(const TRex* tRex,
                                                                     const al::ActorInitInfo& info)
    : mTRex(tRex) {
    sead::Matrix34f identity;
    identity.makeIdentity();

    mBreakModelGroupA = new al::LiveActorGroup("壊れモデルテーブルA", 16);
    mBreakModelMatricesA = new sead::Matrix34f[16];
    for (s32 i = 0; i < 16; i++) {
        mBreakModelMatricesA[i] = identity;
        mBreakModelGroupA->registerActor(
            al::createBreakModel(tRex, info, "壊れモデルA", "TrexBikeExScrollBreakStep000BreakA",
                                 nullptr, &mBreakModelMatricesA[i], "Break"));
    }

    mBreakModelGroupB = new al::LiveActorGroup("壊れモデルテーブルB", 16);
    mBreakModelMatricesB = new sead::Matrix34f[16];
    for (s32 i = 0; i < 16; i++) {
        mBreakModelMatricesB[i] = identity;
        mBreakModelGroupB->registerActor(
            al::createBreakModel(tRex, info, "壊れモデルB", "TrexBikeExScrollBreakStep000BreakB",
                                 nullptr, &mBreakModelMatricesB[i], "Break"));
    }

    mBreakModelGroupC = new al::LiveActorGroup("壊れモデルテーブルC", 16);
    mBreakModelMatricesC = new sead::Matrix34f[16];
    for (s32 i = 0; i < 16; i++) {
        mBreakModelMatricesC[i] = identity;
        mBreakModelGroupC->registerActor(
            al::createBreakModel(tRex, info, "壊れモデルC", "TrexBikeExScrollBreakStep000BreakC",
                                 nullptr, &mBreakModelMatricesC[i], "Break"));
    }
}

void TRexFunction::createTRexScrollBreakMapPartsBreakJudge(const TRex* tRex,
                                                           const al::ActorInitInfo& info) {
    TRexScrollBreakMapPartsBreakJudge* breakJudge =
        new TRexScrollBreakMapPartsBreakJudge(tRex, info);

    return al::setSceneObj(tRex, breakJudge, TRexScrollBreakMapPartsBreakJudge::sSceneObjId);
}
