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
    const char* name = "バルーン";
    const char* layoutName = "BalloonSpeak";
    const char* paneName = "TxtMessage";
    const char* message;
    f32 appearDist = 500.0f;
    f32 killDist = 600.0f;
    const char* archiveName = nullptr;
    f32 yPosOffset = 160.0f;
    s32 playerIndex = -1;
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
    const char* mPaneName;
    f32 mAppearDist;
    f32 mKillDist;
    sead::Vector3f mPosOffset = sead::Vector3f::zero;
    s32 mPlayerIndex;
    s32 _158;
    bool mIsAutoUpdate;
    bool _15d;
    bool mIsTalkMessageVoicePlayerStarted = false;
    TalkMessageVoicePlayer* mTalkMessageVoicePlayer = nullptr;
    sead::FixedSafeString<0x40> mSeName = {};
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
