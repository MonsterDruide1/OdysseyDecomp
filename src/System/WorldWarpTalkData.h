#pragma once

#include <basis/seadTypes.h>

#include "System/ByamlSave.h"

class WorldWarpTalkData : public ByamlSave {
public:
    WorldWarpTalkData(s32);
    void init();
    bool isAlreadyTalkPrevWorld(s32) const;
    bool isAlreadyTalkNextWorld(s32) const;
    void setAlreadyTalkPrevWorld(s32);
    void setAlreadyTalkNextWorld(s32);
    s32 getTipsId() const;
    void addTipsId();
    void setTalkSpecial(bool, bool);
    bool isTalkFindKoopa() const;
    bool isTalkBossRaid() const;
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& iter) override;

private:
    void* _padding[0x4];
};

static_assert(sizeof(WorldWarpTalkData) == 0x28);
