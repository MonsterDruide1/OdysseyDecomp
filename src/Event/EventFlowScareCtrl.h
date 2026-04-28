#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Event/IUseEventFlowData.h"
#include "Library/Nerve/IUseNerve.h"

namespace al {
struct ActorInitInfo;
class EventFlowDataHolder;
class HitSensor;

class EventFlowScareCtrlBase {
public:
    virtual bool isScare() const = 0;
    virtual bool tryGetScareEnemyPos(sead::Vector3f* out) const = 0;
    virtual const char16* getScareMessage() const = 0;
    virtual void update() = 0;
    virtual void attackSensor(HitSensor* self, HitSensor* other) = 0;
};
class LiveActor;
}  // namespace al

class EventFlowScareCtrl : public al::EventFlowScareCtrlBase,
                           public al::IUseEventFlowData,
                           public al::IUseNerve {
public:
    EventFlowScareCtrl();

    void init(al::LiveActor* actor, const al::ActorInitInfo& info,
              al::EventFlowDataHolder* dataHolder, const char* eventName);

    bool isScare() const override;
    bool tryGetScareEnemyPos(sead::Vector3f* out) const override;
    const char16* getScareMessage() const override;
    void update() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    al::EventFlowDataHolder* getEventFlowDataHolder() const override;
    al::NerveKeeper* getNerveKeeper() const override;

    void exeWait();
    void exeScare();
    void exeScareAfter();

    void disableRunAway() { mIsInvalidRunAway = true; }

private:
    al::LiveActor* mActor = nullptr;
    al::NerveKeeper* mNerveKeeper = nullptr;
    al::EventFlowDataHolder* mEventFlowDataHolder = nullptr;
    void* mScareAreaGroup = nullptr;
    s32 mHackType = -1;
    bool _3c = false;
    void* _40 = nullptr;
    void* _48 = nullptr;
    bool _50 = false;
    char _51[0x3] = {};
    void* _54 = nullptr;
    bool mIsInvalidRunAway = false;
    char _61[0x7] = {};
};

static_assert(sizeof(EventFlowScareCtrl) == 0x68);
