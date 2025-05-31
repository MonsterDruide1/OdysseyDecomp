#pragma once

namespace al {
class IUseSceneObjHolder;
}
struct BindInfo;

namespace rs {

bool tryCloseHackTutorial(const al::IUseSceneObjHolder*);
void changeHackTutorialText(const al::IUseSceneObjHolder*, const char*);
void resetHackTutorialText(const al::IUseSceneObjHolder*);
void tryAppearBindTutorial(const al::IUseSceneObjHolder*, const BindInfo&);
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
void tryAppearPlayerGrabPoleTutorial(const al::IUseSceneObjHolder*);
void tryClosePlayerGrabPoleTutorial(const al::IUseSceneObjHolder*);
void tryAppearPlayerClimbPoleTutorial(const al::IUseSceneObjHolder*);
void tryClosePlayerClimbPoleTutorial(const al::IUseSceneObjHolder*);
void tryAppearPlayerClimbPoleTopTutorial(const al::IUseSceneObjHolder*);
void tryClosePlayerClimbPoleTopTutorial(const al::IUseSceneObjHolder*);
void tryAppearKoopaCapTutorial(const al::IUseSceneObjHolder*);
void tryAppearKoopaCapTutorialNoMovie(const al::IUseSceneObjHolder*);
void tryCloseKoopaCapTutorial(const al::IUseSceneObjHolder*);
void changeSeparatePlayTutorial(const al::IUseSceneObjHolder*);
void changeSinglePlayTutorial(const al::IUseSceneObjHolder*);
void changeEnableCapTutorial(const al::IUseSceneObjHolder*);
void changeDisableCapTutorial(const al::IUseSceneObjHolder*);
void tryAppearSignboardTutorial(const al::IUseSceneObjHolder*, const char*);
bool isExistSignboardTutorialMovie(const al::IUseSceneObjHolder*, const char*);
void tryCloseSignboardTutorial(const al::IUseSceneObjHolder*);
void tryAppearAmiiboHelpNpcTutorial(const al::IUseSceneObjHolder*);
void tryAppearAmiiboHelpNpcSeparateTutorial(const al::IUseSceneObjHolder*);
bool tryAppearPlayerCameraSubjectiveTutorial(const al::IUseSceneObjHolder*);
void tryClosePlayerCameraSubjectiveTutorial(const al::IUseSceneObjHolder*);
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
