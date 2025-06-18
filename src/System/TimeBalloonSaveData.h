#pragma once

#include "System/ByamlSave.h"

class TimeBalloonSaveData : public ByamlSave {
public:
    TimeBalloonSaveData();
    void init();
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& iter) override;

private:
    void* _padding[0x3];
};

static_assert(sizeof(TimeBalloonSaveData) == 0x20);
