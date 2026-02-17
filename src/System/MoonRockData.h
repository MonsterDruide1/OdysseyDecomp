#pragma once

#include <basis/seadTypes.h>

#include "System/ByamlSave.h"

class MoonRockData : public ByamlSave {
public:
    MoonRockData(s32);
    bool isEnableShowDemoOpenMoonRockFirst() const;
    bool isEnableShowDemoOpenMoonRockWorld(s32) const;
    void showDemoOpenMoonRockFirst();
    void showDemoOpenMoonRockWorld(s32);
    s32 calcMoonRockTalkMessageIndex() const;
    void addMoonRockTalkMessageIndex(s32);
    bool isAppearedMoonRockTalkMessage(s32) const;
    void showDemoAfterOpenMoonRockFirst();
    bool isEnableShowDemoAfterOpenMoonRockFirst() const;
    bool isEnableShowDemoMoonRockMapWorld(s32) const;
    void showDemoMoonRockMapWorld(s32);
    void init();
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& save) override;

private:
    void* _padding[0x6];
};

static_assert(sizeof(MoonRockData) == 0x38);
