#pragma once

#include "Library/Event/IEventFlowActionNameConverter.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class LiveActor;
struct ActorInitInfo;
}  // namespace al
class NpcStateReactionParam;
class TalkNpcParam;

class TalkNpcActionAnimInfo : public al::HioNode, public al::IEventFlowActionNameConverter {
public:
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

    TalkNpcActionAnimInfo();

    const char* getArgWaitActionName(const al::ActorInitInfo& initInfo);

    void initWaitActionNameFromPlacementInfo(const al::LiveActor* actor,
                                             const al::ActorInitInfo& initInfo, bool);

    void initWaitActionNameDirect(const al::LiveActor* actor, const char*, bool);

    void init(const al::LiveActor* actor, const al::ActorInitInfo& initInfo,
              const TalkNpcParam* npcParam, const char*);

    const char* getWaitActionName() const;

    const char* tryGetActorParamSuffix() const;

    const char* tryApplyVisAnim() const;

    s32 convertActionName(sead::BufferedSafeStringBase<char>*, const char*) const override;

    void changeWaitActionName(const char*, const TalkNpcParam* npcParam);

    void changeHackWaitActionName(const char*, TalkNpcParam* npcParam);

    void onHackWaitActionName(const TalkNpcParam* npcParam);

    void offHackWaitActionName(const TalkNpcParam* npcParam);

    bool changeWaitActionNameBySwitch(const char*, const TalkNpcParam* npcParam);

    bool resetWaitActionNameBySwitch(const TalkNpcParam* npcParam);

    bool isSelectedInitWaitAction() const;

    const char* getAnyRandomActionName() const;

    void setWaitActionName(const char* name) { mWaitActionName = name; }

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
