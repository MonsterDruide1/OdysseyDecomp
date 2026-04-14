#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class QuestInfo;
class SaveObjInfo;

class QuestObj : public al::LiveActor {
public:
    QuestObj(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void switchStart();
    void start();
    void switchStop();
    void initAfterPlacement() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

private:
    QuestInfo* mQuestInfo = nullptr;
    bool mIsQuestStarted = false;
    SaveObjInfo* mSaveObjInfo = nullptr;
    SaveObjInfo* mSwitchStartSaveObjInfo = nullptr;
    bool mIsQuestStopped = false;
};

static_assert(sizeof(QuestObj) == 0x130);
