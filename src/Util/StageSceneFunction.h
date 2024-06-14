#pragma once

namespace al {
class LiveActor;
}

namespace StageSceneFunction {
void appearPlayerDeadCoin(al::LiveActor*);
}

class IJudge;
class BarrierField;

namespace rs {
void resetJudge(IJudge*);
void updateJudge(IJudge*);
bool isJudge(const IJudge*);
bool updateJudgeAndResult(IJudge*);
bool judgeAndResetReturnTrue(IJudge*);
void isEnableShowDemoAfterOpenMoonRockFirst(const al::LiveActor*);
void isFirstDemoOpenMoonRock(const al::LiveActor*);
void isEnableShowDemoMoonRockMapWorld(const al::LiveActor*);
void showDemoAfterOpenMoonRockFirst(const al::LiveActor*);
void showDemoMoonRockMapWorld(const al::LiveActor*);
void setBossBarrierField(BarrierField*);
}  // namespace rs
