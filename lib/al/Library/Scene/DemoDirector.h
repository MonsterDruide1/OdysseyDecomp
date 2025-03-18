#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class LiveActor;
class ActorInitInfo;
class EffectSystem;
class PlacementId;

class DemoDirector : public HioNode {
public:
    DemoDirector(s32);

    void addDemoActorWithSubActor(LiveActor*);
    void addDemoActor(LiveActor*);
    virtual void endInit(const ActorInitInfo&);
    void isActiveDemo() const;
    void getActiveDemoName() const;
    void requestStartDemo(const char*);
    void requestEndDemo(const char*);
    void tryAddDemoActor(LiveActor*);
    void getDemoActorList() const;
    void getDemoActorNum() const;
    virtual void updateDemoActor(EffectSystem*);
    void registDemoRequesterToAddDemoInfo(const PlacementId&);
    void findOrCreateAddDemoInfo(const PlacementId&);
    void registActorToAddDemoInfo(LiveActor*, const PlacementId&);
    void tryFindAddDemoInfo(const PlacementId&) const;
    void findAddDemoInfo(const PlacementId&) const;
    virtual void startDemo(const char*);
    virtual void endDemo(const char*);

private:
    const char* mDemoName;
    void* filler[8];
};
}  // namespace al
