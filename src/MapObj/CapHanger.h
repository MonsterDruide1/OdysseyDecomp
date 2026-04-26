#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al
class CapTargetInfo;
class ItemGenerator;
class SaveObjInfo;

class CapHanger : public al::LiveActor {
public:
    CapHanger(const char*, bool = false);
    void init(const al::ActorInitInfo& info) override;
    void initItem(s32, s32, const al::ActorInitInfo&);
    void switchOn();
    void switchOff();
    void switchKill();
    void initAfterPlacement() override;
    void kill() override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void exeWait();
    void exeKeep();
    void exeRelease();
    bool isKeep(s32) const;
    void setPeachCastleCap(const sead::Vector3f&);

private:
    CapTargetInfo* mCapTargetInfo;
    void* _110;
    s32 _118;
    s32 mAppearItemNum;
    s32 _120;
    ItemGenerator* mItemGenerator;
    bool mIsEmitEffect;
    void* _138;
    SaveObjInfo* _140;
    sead::Matrix34f _148;
    sead::Matrix34f _178;
    bool _1a8;
};

static_assert(sizeof(CapHanger) == 0x1b0);
