#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;
class EventFlowWatchParam;
class HitSensor;
class IUseNerve;
class LiveActor;
}  // namespace al

class TalkNpcParam {
public:
    TalkNpcParam();

    void init(const al::LiveActor* actor, const char* suffix);
    bool isEqualModelName(const al::LiveActor* actor) const;
    bool isEqualSuffixName(const char* suffix) const;
    void* createAndAppendJointLookAtController(al::LiveActor* actor) const;
    void* createJointGroundSmoothController(al::LiveActor* actor) const;
    bool applyMaterialAnimPreset(al::LiveActor* actor, const char* suffix) const;
    const void* getEventFlowActorParam() const;
    const void* findEventFlowActorSuffixParam(const char* suffix) const;
    bool tryInitPartialSklAnim(al::LiveActor* actor) const;
    bool isValidFacialAnim() const;
    void updateFacialAnim(al::LiveActor* actor) const;
    const char* tryGetByeByeBaseJointName(const al::LiveActor* actor) const;
    void getByeByeLocalAxisFront(sead::Vector3f* out) const;
    void calcBirdGlideMtx(sead::Matrix34f* out, const al::LiveActor* actor) const;
    bool isInvalidJointLookSklAnim(const char* animName) const;
    bool isInvalidChangeAllAnimFromWait(const char* animName) const;
    bool isInvalidChangeTurnAnimFromWait(const char* animName) const;
    bool isPlayerWatchDisregard(const al::HitSensor* sensor) const;
    void calcPlayerWatchTrans(sead::Vector3f* out, const al::LiveActor* actor) const;
    bool isInvalidTrampleSensor(const al::HitSensor* sensor) const;
    bool isEnableReactionRestartEvent(const al::IUseNerve* user) const;
    void manualInitLookAtJoint(const char* jointName, const char* baseJointName,
                               const al::LiveActor* actor, const char* suffix);

    const al::LiveActor* getActor() const { return mActor; }

    const al::LiveActor* getEventFlowActor() const {
        return static_cast<const al::LiveActor*>(mEventFlowActor);
    }

    const al::EventFlowWatchParam* getEventFlowWatchParam() const {
        return static_cast<const al::EventFlowWatchParam*>(mEventFlowActor);
    }

private:
    const al::LiveActor* mActor = nullptr;
    const char* mModelName = nullptr;
    const char* mSuffixName = nullptr;
    const void* mEventFlowActor = nullptr;
    void* _20 = nullptr;
    s32 _28 = 0;
    s32 _2c = -1;
    void* _30 = nullptr;
    void* _38 = nullptr;
    void* _40 = nullptr;
    s32 _48 = 0;
    void* _50 = nullptr;
    void* _58 = nullptr;
    void* _60 = nullptr;
    s32 _68 = 0;
    void* _70 = nullptr;
    void* _78 = nullptr;
    void* _80 = nullptr;
};

static_assert(sizeof(TalkNpcParam) == 0x88);
