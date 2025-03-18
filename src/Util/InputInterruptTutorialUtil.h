#pragma once

namespace al {
class IUseSceneObjHolder;
}
class BindInfo;

namespace rs {

bool tryCloseHackTutorial(const al::IUseSceneObjHolder*);
void changeHackTutorialText(const al::IUseSceneObjHolder*, const char*);
void resetHackTutorialText(const al::IUseSceneObjHolder*);
bool tryAppearBindTutorial(const al::IUseSceneObjHolder*, const BindInfo&);
bool tryCloseBindTutorial(const al::IUseSceneObjHolder*);
bool tryCloseObjectTutorial(const al::IUseSceneObjHolder*, const char*);
void appearFirstHackTutorialFrog(const al::IUseSceneObjHolder*);
void appearFirstHackTutorialKoopa(const al::IUseSceneObjHolder*);
bool tryAppearObjectTutorial(const al::IUseSceneObjHolder*, const char*);
bool tryAppearObjectTutorialOpening(const al::IUseSceneObjHolder*, const char*, const char*);
bool tryCloseObjectTutorialOpening(const al::IUseSceneObjHolder*, const char*);
bool tryAppearObjectTutorialHipDrop(const al::IUseSceneObjHolder*);
bool tryCloseObjectTutorialHipDrop(const al::IUseSceneObjHolder*);
bool tryAppearObjectTutorialCapThrow(const al::IUseSceneObjHolder*);
bool tryCloseObjectTutorialCapThrow(const al::IUseSceneObjHolder*);
bool tryAppearPlayerGrabPoleTutorial(const al::IUseSceneObjHolder*);
bool tryClosePlayerGrabPoleTutorial(const al::IUseSceneObjHolder*);
bool tryAppearPlayerClimbPoleTutorial(const al::IUseSceneObjHolder*);
bool tryClosePlayerClimbPoleTutorial(const al::IUseSceneObjHolder*);
bool tryAppearPlayerClimbPoleTopTutorial(const al::IUseSceneObjHolder*);
bool tryClosePlayerClimbPoleTopTutorial(const al::IUseSceneObjHolder*);
bool tryAppearKoopaCapTutorial(const al::IUseSceneObjHolder*);
bool tryAppearKoopaCapTutorialNoMovie(const al::IUseSceneObjHolder*);
bool tryCloseKoopaCapTutorial(const al::IUseSceneObjHolder*);
void changeSeparatePlayTutorial(const al::IUseSceneObjHolder*);
void changeSinglePlayTutorial(const al::IUseSceneObjHolder*);
void changeEnableCapTutorial(const al::IUseSceneObjHolder*);
void changeDisableCapTutorial(const al::IUseSceneObjHolder*);
bool tryAppearSignboardTutorial(const al::IUseSceneObjHolder*, const char*);
bool isExistSignboardTutorialMovie(const al::IUseSceneObjHolder*, const char*);
bool tryCloseSignboardTutorial(const al::IUseSceneObjHolder*);
bool tryAppearAmiiboHelpNpcTutorial(const al::IUseSceneObjHolder*);
bool tryAppearAmiiboHelpNpcSeparateTutorial(const al::IUseSceneObjHolder*);
bool tryAppearPlayerCameraSubjectiveTutorial(const al::IUseSceneObjHolder*);
bool tryClosePlayerCameraSubjectiveTutorial(const al::IUseSceneObjHolder*);
void appearCapMsgTutorial(const al::IUseSceneObjHolder*, const char*);
void closeCapMsgTutorial(const al::IUseSceneObjHolder*);
void appearRaceTutorial(const al::IUseSceneObjHolder*, const char*);
void killRaceTutorial(const al::IUseSceneObjHolder*, const char*);
void appearRaceTutorialTutorial(const al::IUseSceneObjHolder*);
bool isShowTutorial(const al::IUseSceneObjHolder*);
bool isShowObjectTutorial(const al::IUseSceneObjHolder*, const char*);
void hideTutorial(const al::IUseSceneObjHolder*);
void showTutorial(const al::IUseSceneObjHolder*);
void appearTutorialForDemo(const al::IUseSceneObjHolder*);
void killTutorial(const al::IUseSceneObjHolder*);
void updateGyroText(const al::IUseSceneObjHolder*);
}  // namespace rs

namespace TimeBalloonFunction {
void appearTimeBalloonTutorial(const al::IUseSceneObjHolder*);
void closeTimeBalloonTutorial(const al::IUseSceneObjHolder*);
}  // namespace TimeBalloonFunction
