#pragma once

#include <math/seadVector.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
struct ActorInitInfo;
class LayoutInitInfo;
class LiveActor;
class TalkMessageVoicePlayer;

struct BalloonMessageInitParam {
public:
    const char* _0 = "バルーン";
    const char* _8 = "BalloonSpeak";
    const char* _10 = "TxtMessage";
    const char* _18;
    f32 _20 = 500.0f;
    f32 _24 = 600.0f;
    const char* _28 = nullptr;
    f32 _30 = 160.0f;
    s32 _34 = -1;
    s32 _38 = 0;
    bool _3c = false;
};

class BalloonMessage : public LayoutActor {
public:
    static BalloonMessage* create(const LiveActor* hostActor, const LayoutInitInfo& info,
                                  const BalloonMessageInitParam& initParam, bool isAutoUpdate);

    BalloonMessage(const LiveActor* hostActor, const LayoutInitInfo& info,
                   const BalloonMessageInitParam& initParam, bool isAutoUpdate);

    void appear() override;
    void control() override;

    void end();
    void update();
    void updateTrans();
    void appearWithPushA();
    void hidePushA();
    void showPushA();

    void setText(const char* message);
    void setTextW(const char16* message);
    bool isEnableAppear() const;
    bool isEnableEnd() const;
    bool isNearPlayerActor(f32 threshold) const;
    bool isShowPushA() const;
    bool isVoicePlayerPlaying() const;
    bool isWait() const;

    void exeAppear();
    void exeWait();
    void exeEnd();
    void exeHide();

private:
    const LiveActor* mHostActor;
    const char* _138;
    f32 _140;
    f32 _144;
    sead::Vector3f _148 = sead::Vector3f::zero;
    s32 _154;
    s32 _158;
    bool mIsAutoUpdate;
    bool _15d;
    bool _15e = false;
    TalkMessageVoicePlayer* mTalkMessageVoicePlayer = nullptr;
    sead::FixedSafeString<0x40> _168 = {};
};

BalloonMessage* createBalloonMessage(const LiveActor* hostActor, const ActorInitInfo& info,
                                     const BalloonMessageInitParam& initParam);
BalloonMessage* createBalloonMessageNoAutoUpdate(const LiveActor* hostActor,
                                                 const ActorInitInfo& info,
                                                 const BalloonMessageInitParam& initParam);
BalloonMessage* createBalloonMessage(const LiveActor* hostActor, const ActorInitInfo& info);
BalloonMessage* createBalloonMessage(const LiveActor* hostActor, const ActorInitInfo& info,
                                     const char* message);

static_assert(sizeof(BalloonMessageInitParam) == 0x40);
static_assert(sizeof(BalloonMessage) == 0x1c0);
}  // namespace al
