#pragma once

namespace al {
class ActorItemInfo;
class HitSensor;
class LiveActor;
struct ActorInitInfo;

class ActorItemKeeper {
public:
    ActorItemKeeper(const LiveActor*, s32);

    ActorItemInfo* addItem(const ActorInitInfo&, const char*, const char*, const char*, s32, bool);
    void reset();
    void setFactor(const char*, const HitSensor*);
    ActorItemInfo* getAppearItemInfo(const char*) const;

private:
    void* unknown[0x7];
};
}  // namespace al
