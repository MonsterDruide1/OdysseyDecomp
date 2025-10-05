#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

namespace al {
struct ActorInitInfo;
class EffectSystem;
class LiveActor;
class PlacementId;

class AddDemoInfo {
public:
    AddDemoInfo(s32 size);
    void init(const PlacementId& placementId);
    void addDemoActor(LiveActor* actor);
    const char* getDemoName() const;
    void reset();
    LiveActor* getDemoActor(s32 index) const;

private:
    const char* mName = nullptr;
    PlacementId* mPlacementId = nullptr;
    LiveActor** mActorList = nullptr;
    s32 mActorListCount = 0;
    s32 mActorListSize = 0;
};

static_assert(sizeof(AddDemoInfo) == 0x20);

class DemoDirector : public HioNode {
public:
    DemoDirector(s32);

    void addDemoActorWithSubActor(LiveActor*);
    void addDemoActor(LiveActor*);
    virtual void endInit(const ActorInitInfo&);
    bool isActiveDemo() const;
    const char* getActiveDemoName() const;
    bool requestStartDemo(const char*);
    void requestEndDemo(const char*);
    bool tryAddDemoActor(LiveActor*);
    LiveActor** getDemoActorList() const;
    s32 getDemoActorNum() const;
    virtual void updateDemoActor(EffectSystem*);
    AddDemoInfo* registDemoRequesterToAddDemoInfo(const PlacementId&);
    AddDemoInfo* findOrCreateAddDemoInfo(const PlacementId&);
    void registActorToAddDemoInfo(LiveActor*, const PlacementId&);
    AddDemoInfo* tryFindAddDemoInfo(const PlacementId&) const;
    // BUG: only ever returns `nullptr`
    AddDemoInfo* findAddDemoInfo(const PlacementId&) const;
    virtual bool startDemo(const char*);
    virtual void endDemo(const char*);

private:
    const char* mActiveDemoName;
    s32 _10;
    LiveActor** mActorList;
    s32 mActorListCount;
    s32 mActorListSize;
    void* filler[5];
};

static_assert(sizeof(DemoDirector) == 0x50);

}  // namespace al
