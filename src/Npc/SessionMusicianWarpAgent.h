#pragma once

namespace al {
struct ActorInitInfo;
class LiveActor;
class PlacementInfo;
}  // namespace al

class ChangeStageInfo;

class SessionMusicianWarpAgent {
public:
    SessionMusicianWarpAgent(al::LiveActor* actor, const al::ActorInitInfo& initInfo);

    bool tryGetWarpTargetInfo(al::PlacementInfo* targetInfo);
    bool tryStartWarp();

    al::LiveActor* getActor() const { return mActor; }

    ChangeStageInfo* getChangeStageNormal() const { return mChangeStageNormal; }

    ChangeStageInfo* getChangeStageTalk() const { return mChangeStageTalk; }

private:
    al::LiveActor* mActor;
    al::PlacementInfo* mPlacementInfo = nullptr;
    ChangeStageInfo* mChangeStageNormal = nullptr;
    ChangeStageInfo* mChangeStageTalk = nullptr;
};

static_assert(sizeof(SessionMusicianWarpAgent) == 0x20);
