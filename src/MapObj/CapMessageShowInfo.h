#pragma once

#include <basis/seadTypes.h>

namespace al {
class IUseSceneObjHolder;
}  // namespace al

namespace rs {
void showCapMessage(const al::IUseSceneObjHolder*, const char*, s32, s32);
bool isActiveCapMessage(const al::IUseSceneObjHolder*);
bool tryShowCapMsgCollectCoinGetFirst(const al::IUseSceneObjHolder*);
}  // namespace rs
