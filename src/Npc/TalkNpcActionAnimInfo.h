#pragma once

#include "Library/Event/IEventFlowActionNameConverter.h"

namespace al {
class LiveActor;
struct ActorInitInfo;
}  // namespace al
class NpcStateReactionParam;
class TalkNpcParam;

class TalkNpcActionAnimInfo : public al::IEventFlowActionNameConverter {
public:
    TalkNpcActionAnimInfo();

    struct TalkNpcActionAnimNameSet {
        const char* WaitActionName;
        const char* WalkActionName;
        const char* TalkActionName;
        const char* TurnActionName;
        const char* TurnL90ActionName;
        const char* TurnR90ActionName;
        const char* Turn180ActionName;
        const char* ReactionName;
        const char* ReactionCapName;
        const char* GiveActionName;
        const char* ExcitedActionName;
        const char* ScaredStartActionName;
        const char* ScaredActionName;
        const char* ScaredEndActionName;
        const char* VisAnimName;
    };

    void setWaitActionName(const char* name) { mWaitActionName = name; }

    void init(const al::LiveActor*, const al::ActorInitInfo&, const TalkNpcParam*, const char*);

private:
    const al::LiveActor* mActor;
    const TalkNpcActionAnimNameSet* mActionNames;
    NpcStateReactionParam* mReactionParam;

    const char* mWaitActionName;
    const char* mHackingWaitActionName;
    bool mIsUseHackWaitAction;

    const char* mWaitActionNameBySwitch;
    const char* mHackWaitActionName;
    const char* mWaitActionNameFromSwitch;

    s32 mRandomActionCount;
    const char** mRandomActionNames;

    bool mIsInvalidChangeTurnAnimFromWait;
    bool mIsInvalidChangeAllAnimFromWait;
};

static_assert(sizeof(TalkNpcActionAnimInfo) == 0x68);
