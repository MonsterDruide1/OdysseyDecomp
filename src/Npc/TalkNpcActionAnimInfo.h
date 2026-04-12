#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

#include "Util/IEventFlowActionNameConverter.h"

namespace al {
struct ActorInitInfo;
class LiveActor;
}  // namespace al
class NpcStateReactionParam;
class TalkNpcParam;

class TalkNpcActionAnimInfo : public al::HioNode, public al::IEventFlowActionNameConverter {
public:
    TalkNpcActionAnimInfo();

    const char* getArgWaitActionName(const al::ActorInitInfo& info);
    void initWaitActionNameFromPlacementInfo(const al::LiveActor* actor,
                                             const al::ActorInitInfo& info, bool isHackWaitAction);
    void initWaitActionNameDirect(const al::LiveActor* actor, const char* waitActionName,
                                  bool isHackWaitAction);
    void init(const al::LiveActor* actor, const al::ActorInitInfo& info, const TalkNpcParam* param,
              const char* archiveName);
    const char* getWaitActionName() const;
    const char* tryGetActorParamSuffix() const;
    bool tryApplyVisAnim(al::LiveActor* actor) const;
    void convertActionName(sead::BufferedSafeStringBase<char>* outActionName,
                           const char* actionName) const override;
    void changeWaitActionName(const char* waitActionName, const TalkNpcParam* param);
    void changeHackWaitActionName(const char* waitActionName, const TalkNpcParam* param);
    void onHackWaitActionName(const TalkNpcParam* param);
    void offHackWaitActionName(const TalkNpcParam* param);
    void changeWaitActionNameBySwitch(const char* waitActionName, const TalkNpcParam* param);
    void resetWaitActionNameBySwitch(const TalkNpcParam* param);
    bool isSelectedInitWaitAction() const;
    const char* getAnyRandomActionName() const;

    const NpcStateReactionParam* getReactionParam() const { return mReactionParam; }

    void setWaitActionName(const char* waitActionName) { mWaitActionName = waitActionName; }

private:
    const char* mActorParamSuffix = nullptr;
    const char* mInitWaitActionName = nullptr;
    const NpcStateReactionParam* mReactionParam = nullptr;
    const char* mWaitActionName = nullptr;
    const char* mSurpriseActionName = "Surprise";
    bool mIsHackWaitAction = false;
    u8 _31[7] = {};
    const char* _38 = nullptr;
    const char* _40 = nullptr;
    const char* _48 = nullptr;
    s32 _50 = 0;
    const char* _58 = nullptr;
    u16 _60 = 0x101;
    u8 _62[6] = {};
};

static_assert(sizeof(TalkNpcActionAnimInfo) == 0x68);
