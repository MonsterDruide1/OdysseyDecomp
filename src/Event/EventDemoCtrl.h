#pragma once

#include <basis/seadTypes.h>

#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
class LiveActor;
}

enum class EventDemoType : s32 {
    None = 0,
    Normal = 1,
    Talk = 2,
    TalkOnlyRequester = 3,
    TalkKeepHack = 4,
    TalkUseCoin = 5,
    KeepBind = 6,
    CutScene = 7,
    CutSceneKeepHack = 8,
    CutSceneTalkOnlyRequester = 9,
};

struct EventDemoCtrlInfo {
    EventDemoType demoType = EventDemoType::None;
    al::LiveActor* demoStartActor = nullptr;
    const al::LiveActor* lockTalkActor = nullptr;
    u8 isRequestEndDemo = false;
    u8 isDemoSkipStart = false;
};

static_assert(sizeof(EventDemoCtrlInfo) == 0x20);

class EventDemoCtrl : public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_EventDemoCtrl;

    EventDemoCtrl();

    const char* getSceneObjName() const override { return "イベントデモ操作"; }

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

private:
    EventDemoCtrlInfo* mEventDemoInfo = nullptr;
};

static_assert(sizeof(EventDemoCtrl) == 0x10, "EventDemoCtrl");
