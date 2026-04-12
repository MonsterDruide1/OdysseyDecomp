#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Event/IEventFlowEventReceiver.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class EventFlowEventData;
class HitSensor;
class SensorMsg;
}  // namespace al
class ItemGenerator;
class NpcJointLookAtController;
class TalkNpcActionAnimInfo;
class TalkNpcParam;
class TalkNpcStateEvent;

class FishingUpObj : public al::LiveActor, public al::IEventFlowEventReceiver {
public:
    FishingUpObj(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void kill() override;
    void updateCollider() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    bool receiveEvent(const al::EventFlowEventData* event) override;
    bool isKinopioBrigade() const;
    bool isPochi() const;
    bool isExistShine() const;
    bool isAfterGiveShine() const;
    bool isValidAfterAppearDemoEvent() const;
    bool tryStartAppearAndStartDemoEvent();
    bool tryUpdateShineHintTrans(const sead::Vector3f& trans);
    void exeFishingUp();
    void exePopUp();
    void exeLand();
    void exeEvent();

private:
    TalkNpcStateEvent* mTalkNpcStateEvent = nullptr;
    const TalkNpcParam* mTalkNpcParam = nullptr;
    TalkNpcActionAnimInfo* mTalkNpcActionAnimInfo = nullptr;
    ItemGenerator* mItemGenerator = nullptr;
    const char* mItemGeneratePositionJointName = nullptr;
    s32 mLinkedShineIndex = -1;
    NpcJointLookAtController* mJointLookAtController = nullptr;
};

static_assert(sizeof(FishingUpObj) == 0x148);
