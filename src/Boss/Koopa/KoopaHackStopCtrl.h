#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
class LiveActor;
}

class KoopaHackStopCtrl : public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_KoopaHackStopCtrl;

    KoopaHackStopCtrl();

    void startStop(const al::LiveActor* actor);
    void endStop(const al::LiveActor* actor);
    void resetPosture(const al::LiveActor* actor, const sead::Quatf& quat,
                      const sead::Vector3f& trans);
    bool tryResetPosture(al::LiveActor* actor);

    const char* getSceneObjName() const override { return "崩落クッパの停止制御"; }

    bool isStop() const { return mStopActor != nullptr; }

    const al::LiveActor* getStopActor() const { return mStopActor; }

    bool isStatusDemoForSceneKoopaHack() const { return mIsStatusDemoForSceneKoopaHack; }

    void setStatusDemoForSceneKoopaHack(bool status) { mIsStatusDemoForSceneKoopaHack = status; }

private:
    const al::LiveActor* mStopActor = nullptr;
    bool mIsNeedResetPosture = false;
    sead::Quatf mResetQuat = sead::Quatf::unit;
    sead::Vector3f mResetTrans = {0.0f, 0.0f, 0.0f};
    bool mIsStatusDemoForSceneKoopaHack = false;
};

static_assert(sizeof(KoopaHackStopCtrl) == 0x38);

namespace KoopaHackFunction {
void startStopKoopaHack(al::LiveActor* actor);
void endStopKoopaHack(al::LiveActor* actor);
void resetPostureKoopaHack(al::LiveActor* actor, const sead::Quatf& quat,
                           const sead::Vector3f& trans);
bool isStopKoopaHack(const al::LiveActor* actor);
bool isStatusDemoForSceneKoopaHack(const al::LiveActor* actor);
void setStatusDemoForSceneKoopaHack(const al::LiveActor* actor);
void resetStatusDemoForSceneKoopaHack(const al::LiveActor* actor);
}  // namespace KoopaHackFunction
