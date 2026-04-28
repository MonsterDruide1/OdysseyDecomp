#pragma once

#include <basis/seadTypes.h>

#include "Library/Scene/ISceneObj.h"

namespace al {
class LiveActor;
}

class EventDemoCtrl : public al::ISceneObj {
public:
    EventDemoCtrl();

    bool isSuccessLockTalkDemo(const al::LiveActor* actor);
    bool tryLockStartTalkDemo(al::LiveActor* actor);
    bool tryLockStartTalkDemoWithoutBalloon(al::LiveActor* actor);
    bool tryStartTalkDemo(al::LiveActor* actor);
    bool tryStartTalkOnlyRequesterDemo(al::LiveActor* actor);
    bool tryStartTalkKeepHackDemo(al::LiveActor* actor);
    bool tryStartTalkUseCoinDemo(al::LiveActor* actor);
    bool tryStartNormalDemo(al::LiveActor* actor);
    bool tryStartKeepBindDemo(al::LiveActor* actor);
    bool tryStartCutSceneDemo(al::LiveActor* actor);
    bool tryStartCutSceneKeepHackDemo(al::LiveActor* actor);
    bool tryStartCutSceneTalkOnlyRequesterDemo(al::LiveActor* actor);
    void requestEndDemo(al::LiveActor* actor);
    void endCutSceneDemo(al::LiveActor* actor);
    void endCutSceneTalkOnlyRequesterDemo(al::LiveActor* actor);
    void endCutSceneDemoBySkip(al::LiveActor* actor);
    bool isActiveDemo() const;
    bool isActiveDemoWithPlayer() const;
    bool isRequestEndDemo() const;
    al::LiveActor* getDemoStartActor() const;
    void endDemo();
    bool isDemoStartActor(const al::LiveActor* actor) const;
    void notifyStartDemoSkipFromScene();
    bool isDemoSkipStart() const;
    const char* getSceneObjName() const override;

private:
    void* mInfo;
};

static_assert(sizeof(EventDemoCtrl) == 0x10);
