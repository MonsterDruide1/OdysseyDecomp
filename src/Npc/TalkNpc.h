#pragma once

#include <basis/seadTypes.h>

#include "Library/Event/IEventFlowEventReceiver.h"
#include "Library/LiveActor/LiveActor.h"

class BgmAnimeSynchronizer;
class TalkNpcParam;
class TalkNpcStateEvent;

namespace al {
class AreaObjGroup;
class ClippingActorInfo;
class EventFlowEventData;
class IUseEventFlowData;
class MtxConnector;
}  // namespace al

// 0x10-byte heap struct allocated by TalkNpc::init: the NPC's
// ClippingActorInfo plus the near-clip value to restore when the
// Fukankun (overhead) camera setting is turned off. Decoded from
// TalkNpc::init / onFukankunCameraNearClipSetting /
// offFukankunCameraNearClipSetting.
struct TalkNpcFukankunNearClipInfo {
    al::ClippingActorInfo* mClippingActorInfo;
    f32 mSavedNearClip;
};

class TalkNpc : public al::LiveActor, public al::IEventFlowEventReceiver {
public:
    TalkNpc(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void kill() override;
    void movement() override;
    void calcAnim() override;
    void endClipped() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    bool receiveEvent(const al::EventFlowEventData* event) override;
    void updateCollider() override;

    void onFukankunCameraNearClipSetting();
    void offFukankunCameraNearClipSetting();
    TalkNpcStateEvent* tryGetCurrentEventState() const;
    al::IUseEventFlowData* tryGetCurrentEvent() const;
    void startDemoKoopa(const sead::Vector3f& front);
    void endDemoKoopa();
    void exeEvent();
    void exeHackEvent();
    void exeKinopioBrigadeFirstEvent();
    void exeDemoKoopa();

private:
    u8 _110[0x18];
    const TalkNpcParam* mTalkNpcParam;
    TalkNpcStateEvent* _130;
    TalkNpcStateEvent* _138;
    TalkNpcStateEvent* _140;
    u8 _148[0xc];
    s32 _154;
    u8 _158[0x18];
    void* _170;
    void* _178;
    u8 _180[0x20];
    TalkNpcFukankunNearClipInfo* _1a0;
    u8 _1a8[0x8];
    const al::MtxConnector* _1b0;
    sead::Quatf _1b8;
    u8 _1c8[0xc];
    sead::Vector3f _1d4;
    void* _1e0;
    sead::Matrix34f _1e8;
    BgmAnimeSynchronizer* _218;
    void* _220;
    const al::AreaObjGroup* _228;
    u8 _230[0x6];

public:
    bool _236;

private:
    u8 _237;
};

static_assert(sizeof(TalkNpc) == 0x238);
