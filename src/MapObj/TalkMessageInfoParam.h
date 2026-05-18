#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"
#include "Library/Message/IUseMessageSystem.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class LiveActor;
class MessageSystem;
class SensorMsg;
}  // namespace al

class TalkMessageInfoMessageParam;

class TalkMessageInfoParam : public al::IUseHioNode, public al::IUseMessageSystem {
public:
    TalkMessageInfoParam(al::LiveActor* actor, const bool* listenFlag, const bool* inAreaFlag,
                         bool showDisableCap);

    void initState(const al::ActorInitInfo& info, const char16* message);
    void listenOn();
    void listenOff();
    void listenKill();
    void listenAppear();
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
    void kill();
    void invalidateClipping();
    bool isEnableAppear() const;
    bool isInArea() const;
    void onAppear();
    void onAppearEnd();
    bool isShowDisableCap() const;
    const al::MessageSystem* getMessageSystem() const override;

    const char16* getMessage() const { return mMessage; }

    bool isRequestEndForce() const { return mIsRequestEndForce; }

    s32 getPriority() const { return mPriority; }

    bool isKeepDisp() const { return mIsKeepDisp; }

    bool isDelayCancel() const { return mIsDelayCancel; }

    s32 getDelayFrame() const { return mDelayFrame; }

    s32 getEndActionType() const { return mEndActionType; }

    TalkMessageInfoMessageParam* getMessageParam() const { return mMessageParam; }

private:
    al::LiveActor* mActor = nullptr;
    const char16* mMessage = nullptr;
    bool mIsRequestEndForce = false;
    u8 _19[3] = {};
    s32 mPriority = 0;
    u16 _20 = 0;
    u8 _22[6] = {};
    const bool* mListenFlag = nullptr;
    const bool* mInAreaFlag = nullptr;
    u8 _38 = 1;
    bool mIsKeepDisp = false;
    bool mIsDelayCancel = true;
    u8 _3b = 0;
    s32 mDelayFrame = 0;
    u8 _40[0x20] = {};
    s32 mEndActionType = 0;
    bool mIsShowDisableCap = false;
    u8 _65[3] = {};
    s32 _68 = 0;
    u16 _6c = 0;
    u8 _6e[2] = {};
    void* _70 = nullptr;
    TalkMessageInfoMessageParam* mMessageParam = nullptr;
};

static_assert(sizeof(TalkMessageInfoParam) == 0x80);

namespace rs {
const char* getColor(const TalkMessageInfoParam* param);
}
