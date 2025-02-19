#pragma once

namespace al {
class IUseSceneObjHolder;
}  // namespace al

namespace AmiiboFunction {

void tryCreateHelpAmiiboDirector(const al::IUseSceneObjHolder*);
bool isTriggerTouchAmiiboMario(const al::IUseSceneObjHolder*);
bool isTriggerTouchAmiiboPeach(const al::IUseSceneObjHolder*);
bool isTriggerTouchAmiiboKoopa(const al::IUseSceneObjHolder*);
bool isTriggerTouchAmiiboAll(const al::IUseSceneObjHolder*);

}  // namespace AmiiboFunction

namespace rs {

void resetHelpAmiiboState(const al::IUseSceneObjHolder*);
bool isHelpAmiiboMode(const al::IUseSceneObjHolder*);

}  // namespace rs
