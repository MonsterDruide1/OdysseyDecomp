#pragma once

#include <basis/seadTypes.h>

#include "System/ByamlSave.h"

class WorldList;

class SequenceDemoSkipData : public ByamlSave {
public:
    SequenceDemoSkipData(const WorldList* world_list);
    void init();
    void showWorldIntroCamera(s32);
    void showScenarioStartCamera(s32, s32);
    bool isAlreadyShowWorldIntroCamera(s32) const;
    bool isAlreadyShowScenarioStartCamera(s32, s32) const;
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& save) override;

private:
    void* _padding[0x4];
};

static_assert(sizeof(SequenceDemoSkipData) == 0x28);
