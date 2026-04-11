#pragma once

namespace al {
class IUseSceneObjHolder;
}  // namespace al

namespace rs {
void disableOpenWipeForSkipDemo(const al::IUseSceneObjHolder*);
bool isDisableOpenWipeForSkipDemo(const al::IUseSceneObjHolder*);
}  // namespace rs
