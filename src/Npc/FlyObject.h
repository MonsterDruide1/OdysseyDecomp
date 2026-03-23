#pragma once

#include <prim/seadSafeString.h>

#include "Library/LiveActor/LiveActor.h"
#include "Library/Message/IUseMessageSystem.h"

namespace al {
struct ActorInitInfo;
}
class FlyObject;
class Shine;

enum class FukanKunInteractionType : s32 {
    None,
    Message,
    Shine,
};

class WaveMovementController {
public:
    WaveMovementController();
    f32 calcUpdatedYCoord(const sead::Vector3f&);

    f32* getVerticalSpeedPtr() { return &mVerticalSpeed; }

    f32* getVerticalAmplitudePtr() { return &mVerticalAmplitude; }

private:
    f32 mVerticalSpeed = 2.0f;
    f32 mVerticalAmplitude = 350.0f;
    f32 mVerticalAngle = 0.0f;
};

class Behavior {
public:
    virtual void setUp(al::LiveActor* actor) = 0;
    virtual void move(al::LiveActor* actor, f32 speed) = 0;
    virtual ~Behavior() = default;

    WaveMovementController* getWaveController() { return &mWaveController; }

private:
    WaveMovementController mWaveController;
};

class Stationary : public Behavior {
public:
    void setUp(al::LiveActor* actor) override;
    void move(al::LiveActor* actor, f32 speed) override;

private:
    sead::Vector3f mTrans;
};

static_assert(sizeof(Stationary) == 0x20);

class OnRails : public Behavior {
public:
    void setUp(al::LiveActor* actor) override;
    void move(al::LiveActor* actor, f32 speed) override;

private:
    using MoveSyncRail = bool(al::LiveActor*, f32);

    MoveSyncRail* mMoveSyncRail = nullptr;
};

static_assert(sizeof(OnRails) == 0x20);

class FukanKunInteractionEmpty {
public:
    FukanKunInteractionEmpty();

    virtual void init(FlyObject* flyObject, const al::ActorInitInfo& info);
    virtual void setUp(FlyObject* flyObject);
    virtual void control(FlyObject* flyObject);
    virtual al::MessageSystem* getMessageSystem() const;
    virtual void interact(FlyObject* flyObject);
};

class FukanKunInteractionBase : public FukanKunInteractionEmpty {
public:
    FukanKunInteractionBase(s32 displayTime);

    void init(FlyObject* flyObject, const al::ActorInitInfo& info) override;
    void setUp(FlyObject* flyObject) override;
    void control(FlyObject* flyObject) override;

private:
    bool mHasInteraction = true;
    s32 mDisplayTime;
};

class FukanKunMessageHolder : public FukanKunInteractionBase {
public:
    FukanKunMessageHolder();
    void init(FlyObject* flyObject, const al::ActorInitInfo& info) override;
    al::MessageSystem* getMessageSystem() const override;
    void interact(FlyObject* flyObject) override;

private:
    al::MessageSystem* mMessageSystem = nullptr;
    sead::FixedSafeString<64> mCapMsg;
};

static_assert(sizeof(FukanKunMessageHolder) == 0x70);

class FukanKunShineHolder : public FukanKunInteractionBase {
public:
    FukanKunShineHolder();
    void init(FlyObject* flyObject, const al::ActorInitInfo& info) override;
    void interact(FlyObject* flyObject) override;

private:
    Shine* mShine = nullptr;
};

static_assert(sizeof(FukanKunShineHolder) == 0x18);

class FlyObject : public al::LiveActor, public al::IUseMessageSystem {
public:
    FlyObject(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void control() override;

    al::MessageSystem* getMessageSystem() const override;

    void exeFly();
    void exeDisappear();

private:
    Behavior* mBehavior = nullptr;
    f32 mMovementSpeed = 22.0f;
    FukanKunInteractionEmpty* mFukanKunInteraction = nullptr;
};

static_assert(sizeof(FlyObject) == 0x128);
