#pragma once

#include "Library/Bgm/BgmDirectorInitInfo.h"
#include "Library/Se/SeDirectorInitInfo.h"

namespace al {
class AudioSystemInfo;
class Sequence;

struct AudioDirectorInitInfo {
    AudioDirectorInitInfo() {}

    AudioSystemInfo* audioSystemInfo = nullptr;
    Sequence* curSequence = nullptr;
    const char* curStage = nullptr;
    s32 scenarioNo = 0;
    void* _20 = nullptr;
    SeDirectorInitInfo seDirectorInitInfo;
    BgmDirectorInitInfo bgmDirectorInitInfo;
    const char* duckingName = nullptr;
};

}  // namespace al
