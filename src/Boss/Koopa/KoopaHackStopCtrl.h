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
    static constexpr s32 sSceneObjId = SceneObjID_KoopaStopHackCtrl;

    KoopaHackStopCtrl();

    const char* getSceneObjName() const override { return "崩落クッパの停止制御"; }

    void startStop(const al::LiveActor* actor);
    void endStop(const al::LiveActor* actor);
    void resetPosture(const al::LiveActor* actor, const sead::Quatf& quat,
                      const sead::Vector3f& trans);
    bool tryResetPosture(al::LiveActor* actor);

private:
    const al::LiveActor* mStopActor = nullptr;
    bool mIsNeedResetPosture = false;
    sead::Quatf mResetQuat = sead::Quatf::unit;
    sead::Vector3f mResetTrans = sead::Vector3f::zero;
    bool mIsStatusDemoForSceneKoopaHack = false;
};

static_assert(sizeof(KoopaHackStopCtrl) == 0x38, "KoopaHackStopCtrl");

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
