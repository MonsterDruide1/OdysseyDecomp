#pragma once

#include "Library/Nerve/NerveExecutor.h"

namespace al {
class LiveActor;
class Scene;
}  // namespace al
class Shine;

class EventFlowSceneExecuteCtrl : public al::NerveExecutor {
public:
    EventFlowSceneExecuteCtrl();

    void requestEventGetShineDirect(const al::LiveActor* actor, Shine* shine);
    void requestEventOpenBgmList(const al::LiveActor* actor);
    void requestEventOpenShineList(const al::LiveActor* actor);
    void requestEventGetAchievement(const al::LiveActor* actor, const char* achievementName);
    bool isExistRequestGetShineDirect() const;
    bool isExistRequestOpenBgmList() const;
    bool isExistRequestOpenShineList() const;
    bool isExistRequestGetAchievement() const;
    Shine* getShine() const;
    const char* getAchievementName() const;
    void startSceneExecute(const al::Scene* scene);
    void endSceneExecute(const al::Scene* scene);
    bool isExecuteScene() const;
    bool checkEndSceneExecuteAndReset(const al::LiveActor* actor);

private:
    void* mRequestActor = nullptr;
    void* mRequestArg = nullptr;
};

static_assert(sizeof(EventFlowSceneExecuteCtrl) == 0x20);
