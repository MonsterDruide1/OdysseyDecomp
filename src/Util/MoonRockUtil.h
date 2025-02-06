#pragma once

namespace al {
class LiveActor;
}

namespace rs {
bool isEnableShowDemoAfterOpenMoonRockFirst(const al::LiveActor* actor);
bool isFirstDemoOpenMoonRock(const al::LiveActor* actor);
bool isEnableShowDemoMoonRockMapWorld(const al::LiveActor* actor);
void showDemoAfterOpenMoonRockFirst(const al::LiveActor* actor);
void showDemoMoonRockMapWorld(const al::LiveActor* actor);
}  // namespace rs
