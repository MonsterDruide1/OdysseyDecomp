#pragma once

#include <container/seadPtrArray.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class EventFlowEventData;
class HitSensor;
class SensorMsg;
}  // namespace al

class SessionMusicianNpc;

class SessionMayorNpc : public al::LiveActor {
public:
    void init(const al::ActorInitInfo& info) override;
    void movement() override;
    void initIntroductionCamera(const al::ActorInitInfo& info,
                                sead::PtrArray<SessionMusicianNpc>* musicians);
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    bool receiveEvent(const al::EventFlowEventData* data);
    bool judgeQuery(const char* query) const;
    void control() override;
    void startReaction();
    void startReactionOnlyTurn();
    void startReactionOnlyFlower();
    bool tryStartReaction();
    bool tryAppearMemberMusicians();
    bool tryStartDance();
    void exeWait();
    void exeReaction();
    void exeDance();
    void exeDemo();
};
