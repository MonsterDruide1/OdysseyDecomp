#pragma once

#include <basis/seadTypes.h>

namespace al {
class IUseSceneObjHolder;
}  // namespace al

namespace rs {
s32 getActiveQuestNum(const al::IUseSceneObjHolder*);
s32 getActiveQuestNo(const al::IUseSceneObjHolder*);
const char* getActiveQuestLabel(const al::IUseSceneObjHolder*);
}  // namespace rs
