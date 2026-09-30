#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>

#include "Library/Nerve/NerveStateBase.h"

class IUseDemoSkip;

namespace al {
struct ActorInitInfo;
class DemoActorHolder;
class LiveActor;
}  // namespace al

class SimpleDemoState : public al::ActorStateBase {
public:
    SimpleDemoState(al::LiveActor* actor, const al::ActorInitInfo& info, const char* demoName,
                    const sead::Matrix34f* baseMtx);

    void appear() override;
    virtual bool requestStartDemo();
    virtual void requestEndDemo();
    virtual bool startDemoProc();
    virtual void endDemoProcBeforeRequest();
    virtual void endDemoProc();

    void exeDemo();
    bool tryStartDemo();
    bool tryEndDemo();

protected:
    al::DemoActorHolder* mDemoActorHolder = nullptr;
    bool mIsDemoStarted = true;
    bool mIsInvalidateClipping = false;
};

static_assert(sizeof(SimpleDemoState) == 0x30);

class SimpleDemoStateSkip : public SimpleDemoState {
public:
    SimpleDemoStateSkip(al::LiveActor* actor, const al::ActorInitInfo& info, const char* demoName,
                        const sead::Matrix34f* baseMtx, IUseDemoSkip* demoSkipRequester,
                        bool isUsePlayer, bool isHideDemoPlayer)
        : SimpleDemoState(actor, info, demoName, baseMtx), mDemoSkipRequester(demoSkipRequester),
          mIsUsePlayer(isUsePlayer), mIsHideDemoPlayer(isHideDemoPlayer) {}

    void updateOnlyDemoGraphics();
    bool requestStartDemo() override;
    void requestEndDemo() override;

protected:
    IUseDemoSkip* mDemoSkipRequester = nullptr;
    bool mIsUsePlayer = false;
    bool mIsHideDemoPlayer = false;
};

static_assert(sizeof(SimpleDemoStateSkip) == 0x40);
