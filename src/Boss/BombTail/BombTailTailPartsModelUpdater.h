#pragma once

namespace al {
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

class BombTailTailPartsModelUpdater {
public:
    BombTailTailPartsModelUpdater();
    void initPartsFixFileNoRegister(al::LiveActor* actor, al::LiveActor* parent_actor,
                                    const char* suffix, const char* archive_name);
    void preCalcAnim();
    void updatePose();
    void postEndClipped();
    void attackSensor(al::HitSensor* self, al::HitSensor* other);
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
    void offSyncAppearAndHide();
    void onSyncAppearAndHide();

private:
    char filler[0x48];
};

static_assert(sizeof(BombTailTailPartsModelUpdater) == 0x48);
