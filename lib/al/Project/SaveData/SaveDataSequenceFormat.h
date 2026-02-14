#pragma once

#include <basis/seadTypes.h>

#include "Project/SaveDataSequenceBase.h"

namespace al {

class SaveDataSequenceFormat : public SaveDataSequenceBase {
public:
    SaveDataSequenceFormat();

    s32 threadFunc(const char* filename) override;

    // TODO: Rename parameters
    void start(s32 a, s32 b);

private:
    s32 _8 = 0;
    s32 _c = 0;
};

static_assert(sizeof(SaveDataSequenceFormat) == 0x10);

}  // namespace al
