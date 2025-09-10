#pragma once

#include <basis/seadTypes.h>

#include "Library/Camera/IUseCamera.h"
#include "Library/Collision/IUseCollision.h"
#include "Library/Execute/IUseExecutor.h"
#include "Library/Message/IUseMessageSystem.h"
#include "Library/Nerve/NerveExecutor.h"
#include "Library/Scene/IUseSceneObjHolder.h"

namespace al {
struct ActorInitInfo;
class CameraDirector;
class CollisionDirector;
class PlayerHolder;
class MessageSystem;
class SceneObjHolder;
class WipeSimple;
}  // namespace al
class EventDemoCtrl;
class EventFlowSceneExecuteCtrl;
class NpcEventBalloonInfo;
class NpcEventCtrlInfo;
class NpcEventSceneInfo;
class NpcEventTalkInfo;

class NpcEventDirector : public al::NerveExecutor,
                         public al::IUseCamera,
                         public al::IUseCollision,
                         public al::IUseExecutor,
                         public al::IUseMessageSystem,
                         public al::IUseSceneObjHolder {
public:
    NpcEventDirector(const al::PlayerHolder*, al::CameraDirector*, al::CollisionDirector*,
                     const al::MessageSystem*, EventFlowSceneExecuteCtrl*);
    void init(const al::ActorInitInfo& initInfo);
    void execute() override;
    void updateBalloon();
    void killAllBalloonForSnapshotMode();
    void exeWait();
    void exeDemo();
    void exeDemoWipeClose();
    void exeDemoWipeOpen();
    void exeDemoTalk();
    void endDemoTalk();
    void exeDemoTalkEnd();
    void exeDemoSelectChoiceStart();
    void exeDemoSelectChoice();

private:
    NpcEventSceneInfo* mNpcEventSceneInfo;
    NpcEventCtrlInfo* mNpcEventCtrlInfo;
    void* _48;
    void* _50;
    EventDemoCtrl* mEventDemoCtrl;
    EventFlowSceneExecuteCtrl* mEventFlowSceneExecuteCtrl;
    al::WipeSimple* mWipeSimple;
    s32 mWipeFrames;
    NpcEventBalloonInfo* mNpcEventBalloonInfo;
    NpcEventTalkInfo* mNpcEventTalkInfo;
    const al::PlayerHolder* mPlayerHolder;
    al::CameraDirector* mCameraDirector;
    al::CollisionDirector* mCollisionDirector;
    al::SceneObjHolder* mSceneObjHolder;
    const al::MessageSystem* mMessageSystem;
};

static_assert(sizeof(NpcEventDirector) == 0xb0);
