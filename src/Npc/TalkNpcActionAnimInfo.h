#pragma once

#include <basis/seadTypes.h>

#include "Library/Event/IEventFlowActionNameConverter.h"
#include "Library/HostIO/HioNode.h"

namespace al {
struct ActorInitInfo;
class LiveActor;
}  // namespace al
class TalkNpcParam;

class TalkNpcActionAnimInfo : public al::HioNode, public al::IEventFlowActionNameConverter {
public:
    TalkNpcActionAnimInfo();

    const char* getArgWaitActionName(const al::ActorInitInfo& info);
    void initWaitActionNameFromPlacementInfo(const al::LiveActor* actor,
                                             const al::ActorInitInfo& info, bool isHack);
    void initWaitActionNameDirect(const al::LiveActor* actor, const char* actionName, bool isHack);
    void init(const al::LiveActor* actor, const al::ActorInitInfo& info, const TalkNpcParam* param,
              const char* suffix);
    const char* getWaitActionName() const;
    const char* tryGetActorParamSuffix() const;
    bool tryApplyVisAnim(al::LiveActor* actor) const;
    void convertActionName(sead::BufferedSafeString* out, const char* actionName) const override;
    void changeWaitActionName(const char* actionName, const TalkNpcParam* param);
    void changeHackWaitActionName(const char* actionName, const TalkNpcParam* param);
    void onHackWaitActionName(const TalkNpcParam* param);
    void offHackWaitActionName(const TalkNpcParam* param);
    void changeWaitActionNameBySwitch(const char* actionName, const TalkNpcParam* param);
    void resetWaitActionNameBySwitch(const TalkNpcParam* param);
    bool isSelectedInitWaitAction() const;
    const char* getAnyRandomActionName() const;

private:
    void* _8 = nullptr;
    void* _10 = nullptr;
    void* _18 = nullptr;
    void* _20 = nullptr;
    const char* mDefaultActionName = "Surprise";
    bool _30 = false;
    char _31[0x7] = {};
    void* _38 = nullptr;
    void* _40 = nullptr;
    void* _48 = nullptr;
    s32 _50 = 0;
    void* _58 = nullptr;
    u16 _60 = 0x101;
};

static_assert(sizeof(TalkNpcActionAnimInfo) == 0x68);
