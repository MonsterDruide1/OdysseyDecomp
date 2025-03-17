#pragma once

namespace al {
class IUseSceneObjHolder;
}
class BindInfo;

namespace rs {

bool tryCloseHackTutorial(const al::IUseSceneObjHolder*);
void changeHackTutorialText(const al::IUseSceneObjHolder*, char const*);
void resetHackTutorialText(const al::IUseSceneObjHolder*);
bool tryAppearBindTutorial(const al::IUseSceneObjHolder*, const BindInfo&);
bool tryCloseBindTutorial(const al::IUseSceneObjHolder*);
bool tryCloseObjectTutorial(const al::IUseSceneObjHolder*, char const*);
void appearFirstHackTutorialFrog(const al::IUseSceneObjHolder*);
void appearFirstHackTutorialKoopa(const al::IUseSceneObjHolder*);
bool tryAppearObjectTutorial(const al::IUseSceneObjHolder*, char const*);
bool tryAppearObjectTutorialOpening(const al::IUseSceneObjHolder*, char const*, char const*);
bool tryCloseObjectTutorialOpening(const al::IUseSceneObjHolder*, char const*);
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
bool tryAppearSignboardTutorial(const al::IUseSceneObjHolder*, char const*);
bool isExistSignboardTutorialMovie(const al::IUseSceneObjHolder*, char const*);
bool tryCloseSignboardTutorial(const al::IUseSceneObjHolder*);
bool tryAppearAmiiboHelpNpcTutorial(const al::IUseSceneObjHolder*);
bool tryAppearAmiiboHelpNpcSeparateTutorial(const al::IUseSceneObjHolder*);
bool tryAppearPlayerCameraSubjectiveTutorial(const al::IUseSceneObjHolder*);
bool tryClosePlayerCameraSubjectiveTutorial(const al::IUseSceneObjHolder*);
void appearCapMsgTutorial(const al::IUseSceneObjHolder*, char const*);
void closeCapMsgTutorial(const al::IUseSceneObjHolder*);
void appearRaceTutorial(const al::IUseSceneObjHolder*, char const*);
void killRaceTutorial(const al::IUseSceneObjHolder*, char const*);
void appearRaceTutorialTutorial(const al::IUseSceneObjHolder*);
bool isShowTutorial(const al::IUseSceneObjHolder*);
bool isShowObjectTutorial(const al::IUseSceneObjHolder*, char const*);
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
