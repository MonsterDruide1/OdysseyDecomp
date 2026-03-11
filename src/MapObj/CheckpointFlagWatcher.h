#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Camera/IUseCamera.h"
#include "Library/Execute/IUseExecutor.h"
#include "Library/HostIO/HioNode.h"
#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
class LiveActor;
class PlacementInfo;
class Scene;
class SimpleLayoutAppearWaitEnd;
}  // namespace al

class CheckpointFlag;

class CheckpointFlagWatcher : public al::HioNode,
                              public al::ISceneObj,
                              public al::IUseCamera,
                              public al::IUseExecutor {
public:
    static constexpr s32 sSceneObjId = SceneObjID_CheckpointFlagWatcher;

    CheckpointFlagWatcher(al::CameraDirector* director);

    void initStageInfo(const char* stageName, s32 scenarioNum);
    void initAfterPlacementSceneObj(const al::ActorInitInfo& info) override;
    void execute() override;

    void registerCheckpointFlag(CheckpointFlag* checkpoint);
    void setTouchCheckpointFlag(CheckpointFlag* checkpoint);
    void setTouchAfterCheckpointFlag(const char* checkpointName);
    CheckpointFlag* findCheckpointFlag(const char* checkpointName) const;
    CheckpointFlag* tryFindCheckpointFlag(const char* checkpointName) const;
    bool tryFindCheckpointFlagTrans(sead::Vector3f* trans, const char* checkpointName) const;
    void requestShowNameLayout(const char16*);
    void requestCancelNameLayoutByShineGetDemo();
    void requestShowBalloon(const CheckpointFlag*, const sead::Vector3f&);
    void requestHideBalloon();
    void hideBalloonAllAtBossSequence(const al::LiveActor*);
    void showBalloonAllAfterBossSequence(const al::LiveActor*);

    const char* getSceneObjName() const override { return "中間ポイント監視"; }

    ~CheckpointFlagWatcher() override = default;

    al::CameraDirector* getCameraDirector() const override { return mCameraDirector; }

private:
    CheckpointFlag* mLastCheckpoint = nullptr;
    al::DeriveActorGroup<CheckpointFlag>* mCheckpointTable;
    char _28[0x8];
    al::SimpleLayoutAppearWaitEnd* mNameLayout = nullptr;
    bool mIsCancelNameLayout = false;
    char _40[0x30];
    const char* mStageName = nullptr;
    s32 mScenarioNum = 1;
    al::CameraDirector* mCameraDirector = nullptr;
    al::LiveActor* mUnknownActor = nullptr;
};

namespace rs {
void registerCheckpointFlagToWatcher(CheckpointFlag*);
void setTouchCheckpointFlagToWatcher(CheckpointFlag*);
void setTouchAfterCheckpointFlagToWatcher(const al::IUseSceneObjHolder* sceneObjHolder,
                                          const char* checkpointName);
const al::PlacementInfo*
tryFindCheckpointFlagPlayerRestartInfo(const al::IUseSceneObjHolder* sceneObjHolder,
                                       const char* checkpointName);
CheckpointFlag* tryFindCheckpointFlag(const al::IUseSceneObjHolder* sceneObjHolder,
                                      const char* checkpointName);
bool tryFindCheckpointFlagTrans(sead::Vector3f*, const al::IUseSceneObjHolder* sceneObjHolder,
                                const char* checkpointName);
void requestShowCheckpointFlagNameLayout(const CheckpointFlag* checkpoint, const char16* name);
void requestShowCheckpointFlagBalloon(const CheckpointFlag*, const sead::Vector3f&);
void requestHideCheckpointFlagBalloon(const CheckpointFlag*);
void requestCancelCheckpointFlagNameLayoutByShineGetDemo(const al::Scene*);
void hideCheckpointFlagBalloonAllAtBossSequence(const al::LiveActor*);
void showCheckpointFlagBalloonAllAfterBossSequence(const al::LiveActor*);
}  // namespace rs

static_assert(sizeof(CheckpointFlagWatcher) == 0x90);
