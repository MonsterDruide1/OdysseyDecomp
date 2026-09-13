#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

namespace al {
class IUseSceneObjHolder;
}  // namespace al

class InformationAreaData;
class InformationWindowRequester;

enum TutorialType {
    TutorialType_Object = 14,
    TutorialType_None = 19,
};

struct TutorialInfo {
    const al::IUseSceneObjHolder* sceneObjHolder = nullptr;
    InformationWindowRequester* requester = nullptr;
    TutorialType type = TutorialType_None;
    sead::FixedSafeString<128> name;
    bool hasMovieFile = false;
    bool isAutoShow = false;
    u8 updateTextFlag = 1;
    u8 _b3 = 0;
    u8 _b4 = 0;
    u8 _b5 = 0;
    bool isRegistered = false;
    bool isHide = false;
    const char* textSuffix = nullptr;
    InformationAreaData* areaData = nullptr;
};

static_assert(sizeof(TutorialInfo) == 0xc8);
