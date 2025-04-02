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
    bool requestEndDemo(const char*);
    bool tryAddDemoActor(LiveActor*);
    void* getDemoActorList() const;  // unknown return type
    s32 getDemoActorNum() const;
    virtual void updateDemoActor(EffectSystem*);
    void registDemoRequesterToAddDemoInfo(const PlacementId&);
    AddDemoInfo* findOrCreateAddDemoInfo(const PlacementId&);
    void registActorToAddDemoInfo(LiveActor*, const PlacementId&);
    AddDemoInfo* tryFindAddDemoInfo(const PlacementId&) const;
    AddDemoInfo* findAddDemoInfo(const PlacementId&) const;
    virtual bool startDemo(const char*);
    virtual void endDemo(const char*);

private:
    const char* mDemoName;
    void* filler[8];
};
}  // namespace al
