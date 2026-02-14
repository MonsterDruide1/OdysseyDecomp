#pragma once

#include <basis/seadTypes.h>

#include "System/ByamlSave.h"

namespace al {
class ByamlIter;
class ByamlWriter;
}  // namespace al

class WorldWarpTalkData : public ByamlSave {
public:
    WorldWarpTalkData(s32);
    void init();
    bool isAlreadyTalkPrevWorld(s32 worldId) const;
    bool isAlreadyTalkNextWorld(s32 worldId) const;
    void setAlreadyTalkPrevWorld(s32 worldId);
    void setAlreadyTalkNextWorld(s32 worldId);
    s32 getTipsId() const;
    void addTipsId();
    void setTalkSpecial(bool isTalkFindKoopa, bool isTalkBossRaid);
    bool isTalkFindKoopa() const;
    bool isTalkBossRaid() const;
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& save) override;

private:
    s32 mSize;
    bool* mPrev;  // Array of bools of size mSize
    bool* mNext;  // Array of bools of size mSize
    s32 mTipsId;
    bool mIsTalkFindKoopa;
    bool mIsTalkBossRaid;
};

static_assert(sizeof(WorldWarpTalkData) == 0x28);
