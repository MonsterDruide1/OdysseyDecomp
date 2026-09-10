#pragma once

namespace al {
class LiveActor;
struct ActorInitInfo;
}  // namespace al
class RandomActionUpdater;
class TalkNpcActionAnimInfo;
class TalkNpcParam;

class RandomWaitActionUpdater {
public:
    RandomWaitActionUpdater(al::LiveActor* actor, const al::ActorInitInfo& initInfo,
                            const TalkNpcParam* talkNpc, const char* actionName, const char*);

    void setDisableBalloonAction();

    void setRandomOutbreakProbability(f32 probability);

    void update();

private:
    TalkNpcActionAnimInfo* mTalkNpcActionAnimInfo;
    RandomActionUpdater* mRandomActionUpdater;
};

static_assert(sizeof(RandomWaitActionUpdater) == 0x10);
