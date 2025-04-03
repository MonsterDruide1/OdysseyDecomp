#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class AddDemoInfo;
class ActorInitInfo;
class EffectSystem;
class LiveActor;
class PlacementId;

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
    void* filler[8];
};
}  // namespace al
