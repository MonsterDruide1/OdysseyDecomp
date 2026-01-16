#pragma once

#include "Library/Bgm/BgmDirectorInitInfo.h"
#include "Library/Se/SeDirectorInitInfo.h"

namespace al {
struct AudioSystemInfo;
class AreaObjDirector;
class DemoDirector;
class Sequence;

struct AudioDirectorInitInfo {
    AudioSystemInfo* audioSystemInfo = nullptr;
    AreaObjDirector* areaObjDirector = nullptr;
    const char* curStage = nullptr;
    s32 scenarioNo = 0;
    DemoDirector* demoDirector = nullptr;
    SeDirectorInitInfo seDirectorInitInfo;
    BgmDirectorInitInfo bgmDirectorInitInfo;
    const char* duckingName = nullptr;
};

}  // namespace al
