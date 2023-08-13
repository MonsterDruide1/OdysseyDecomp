#pragma once

namespace al {
class IUseSceneObjHolder;
}  // namespace al

namespace rs {
int getActiveQuestNum(const al::IUseSceneObjHolder*);
int getActiveQuestNo(const al::IUseSceneObjHolder*);
const char* getActiveQuestLabel(const al::IUseSceneObjHolder*);
}  // namespace rs
