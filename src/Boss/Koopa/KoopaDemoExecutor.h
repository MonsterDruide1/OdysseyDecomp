#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Camera/CameraPoseInfo.h"

namespace al {
class AddDemoInfo;
class EventFlowExecutor;
class LiveActor;
struct ActorInitInfo;
}  // namespace al
class CaptionInfoHolder;
class ChurchDoor;
class EventActorStateDemoTalk;
class IUseDemoSkip;
class KoopaCameraCtrl;
class KoopaCap;
class KoopaShip;
class Peach;
class TalkNpc;

// Pose (rotation + translation) for the demo CapManHero, created in init().
// Quat defaults to unit, trans to zero; getQuat/getTrans then fill them
// from the placement info.
struct KoopaDemoPose {
    sead::Quatf mQuat = sead::Quatf::unit;
    sead::Vector3f mTrans = {0.0f, 0.0f, 0.0f};
};

// List of demo actors whose LOD models are invalidated while a demo runs
// and validated again when it ends (see update()).
struct KoopaDemoActorArray {
    al::LiveActor** mActors = nullptr;
    s32 mNum = 0;
};

// Member offsets verified from init/initLv1/initLv2/initMoonChurch/
// startDemoAction/update/tryStart*/killAll disassembly:
// 0x00 demo name, 0x08 actor, 0x10 event flow executor, 0x18 demo actor
// array, 0x20 camera ctrl, 0x28/0x30 EventActorStateDemoTalk, 0x38 ground
// actor, 0x40/0x48 demo ground actors, 0x50 demo actor, 0x58 cap model
// actor, 0x60 KoopaShip, 0x68 KoopaCap, 0x70 Peach, 0x78 ChurchDoor,
// 0x80 KoopaCap 2 / demo peach ring actor, 0x88 peach ring rope actor,
// 0x90 church hole actor, 0x98 demo CapManHero, 0xa0 demo CapManHeroine,
// 0xa8 demo pillar actor, 0xb0 demo basement fall actor, 0xb8 demo Koopa
// model actor, 0xc0/0xc8 church standing NPC array + count, 0xd0 demo pose,
// 0xd8/0xe0/0xe8/0xf0/0xf8/0x100 demo poses, 0x108 camera pose info,
// 0x110 demo flags, 0x112/0x113/0x114/0x115 demo state flags,
// 0x118 caption info holder.
// The members at 0xcc (s32) and 0x116 (u16) have no initializer: the ctor
// zero-fills [0x00, 0xcc) and [0xd0, 0x116) with memset and stores 0 to
// 0x118 separately, leaving those two fields untouched.
class KoopaDemoExecutor {
public:
    KoopaDemoExecutor();

    void init(al::LiveActor* actor, const al::ActorInitInfo& info,
              al::EventFlowExecutor* event, KoopaCameraCtrl* cameraCtrl, Peach* peach);
    void initLv1(const al::ActorInitInfo& info, al::LiveActor* actor, KoopaCap* cap,
                 KoopaShip* ship);
    void initMoonChurch(const al::ActorInitInfo& info, ChurchDoor* door, al::LiveActor* actor);
    void initLv2(const al::ActorInitInfo& info, al::LiveActor* actor, KoopaCap* cap,
                 KoopaCap* cap2);
    void registerDemoModel(al::LiveActor* actor);
    bool update();
    void skip();
    void start(const char* demoName);
    void startDemoAction(const char* actionName, bool isResetPlayerDynamics);
    void killAll();
    bool tryStartChurchEnterDemo(IUseDemoSkip* demoSkip, al::AddDemoInfo* demoInfo);
    bool tryStartChurchStartDemo(IUseDemoSkip* demoSkip, al::AddDemoInfo* demoInfo);
    bool tryStartBattleStartDemo(IUseDemoSkip* demoSkip);
    bool tryStartBattleEndDemo(IUseDemoSkip* demoSkip);
    bool tryStartClashBasementDemo(IUseDemoSkip* demoSkip);

private:
    const char* mDemoName = nullptr;
    al::LiveActor* mActor = nullptr;
    al::EventFlowExecutor* mEventFlowExecutor = nullptr;
    KoopaDemoActorArray* mDemoActorArray = nullptr;
    KoopaCameraCtrl* mCameraCtrl = nullptr;
    EventActorStateDemoTalk* mDemoTalk = nullptr;
    EventActorStateDemoTalk* mDemoTalk2 = nullptr;
    al::LiveActor* mGroundActor = nullptr;
    al::LiveActor* mDemoGroundActor = nullptr;
    al::LiveActor* mDemoGroundBreakActor = nullptr;
    al::LiveActor* mDemoActor = nullptr;
    al::LiveActor* mCapModelActor = nullptr;
    KoopaShip* mKoopaShip = nullptr;
    KoopaCap* mKoopaCap = nullptr;
    Peach* mPeach = nullptr;
    ChurchDoor* mChurchDoor = nullptr;
    al::LiveActor* mKoopaCap2 = nullptr;
    al::LiveActor* mPeachRingRopeActor = nullptr;
    al::LiveActor* mChurchHoleActor = nullptr;
    al::LiveActor* mDemoCapManHero = nullptr;
    al::LiveActor* mDemoCapManHeroine = nullptr;
    al::LiveActor* mDemoPillarActor = nullptr;
    al::LiveActor* mDemoBasementFallActor = nullptr;
    al::LiveActor* mDemoKoopaModelActor = nullptr;
    TalkNpc** mChurchStandingNpcs = nullptr;
    s32 mChurchStandingNpcNum = 0;
    s32 _cc;
    KoopaDemoPose* mDemoPose = nullptr;
    KoopaDemoPose* mBattleStartPlayerPose = nullptr;
    KoopaDemoPose* mDemoEnterChurchPlayerPose = nullptr;
    KoopaDemoPose* mPlayerTransAfterDemoEndPose = nullptr;
    KoopaDemoPose* mDemoEndAfterKoopaPose = nullptr;
    KoopaDemoPose* mDemoEndAfterPeachPose = nullptr;
    KoopaDemoPose* mChurchDoorPose = nullptr;
    al::CameraPoseInfo* mCameraPoseInfo = nullptr;
    union {
        u16 mField110U16 = 0;  // 0x110
        struct {
            u8 mField110;  // 0x110
            bool mIsSkip;  // 0x111
        };
    };
    u8 mField112 = 0;
    u8 mField113 = 0;
    u8 mField114 = 0;
    bool mField115 = false;
    u16 _116;
    CaptionInfoHolder* mCaptionInfoHolder = nullptr;
};

static_assert(sizeof(KoopaDemoExecutor) == 0x120);
