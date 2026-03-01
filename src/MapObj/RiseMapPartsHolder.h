#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class AddDemoInfo;
class CameraTicket;
class HitSensor;
class LiveActorGroup;
class SensorMsg;

template <class T>
class DeriveActorGroup;
}  // namespace al
class RiseMapParts;
class SaveObjInfo;

class RiseMapPartsHolder : public al::LiveActor {
public:
    RiseMapPartsHolder(const char* name);
    void init(const al::ActorInitInfo& info) override;
    void startRise();
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void exeWait();
    void exeMove();
    void startRiseAllChild();
    void exeMoveEnd();
    void exeStop();
    void exeRequestDemo();
    void exeDelay();
    void startDemo(bool unk);

    void setSwitchActor(al::LiveActor* actor) { mSwitchActor = actor; }

    void setRiseMapPartsArray(RiseMapParts** riseMapPartsArray, s32 num) {
        mRiseMapPartsArray = riseMapPartsArray;
        mRiseMapPartsNum = num;
    }

private:
    SaveObjInfo* _108 = nullptr;
    SaveObjInfo* _110 = nullptr;
    SaveObjInfo* mSwitchOnSaveInfo = nullptr;
    al::CameraTicket* mDemoCamera = nullptr;
    al::DeriveActorGroup<RiseMapParts>* mRiseMapPartsGroup = nullptr;
    sead::Matrix34f mPlayerRestartPosMtx;
    bool mHasRestartPos = false;
    s32 mDelayFrame = -1;
    bool mIsSwitch = false;
    al::LiveActor* mSwitchActor = nullptr;
    RiseMapParts** mRiseMapPartsArray = nullptr;
    s32 mRiseMapPartsNum = 0;
    s32 mLinkedShineIndex = -1;
    bool _188 = true;
    bool mIsResetFirstPosInMiniGame = false;
    s32 mRelatedBoss = -1;
    al::AddDemoInfo* mAddDemoInfo = nullptr;
};

static_assert(sizeof(RiseMapPartsHolder) == 0x198);
