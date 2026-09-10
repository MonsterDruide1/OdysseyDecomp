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
        const char* waitActionName;
        const char* walkActionName;
        const char* talkActionName;
        const char* turnActionName;
        const char* turnL90ActionName;
        const char* turnR90ActionName;
        const char* turn180ActionName;
        const char* reactionName;
        const char* reactionCapName;
        const char* giveActionName;
        const char* excitedActionName;
        const char* scaredStartActionName;
        const char* scaredActionName;
        const char* scaredEndActionName;
        const char* visAnimName;
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

    s32 convertActionName(sead::BufferedSafeString*, const char*) const override;

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
