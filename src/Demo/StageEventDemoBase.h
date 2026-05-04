#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

#include "Demo/IUseDemoSkip.h"

class SaveObjInfo;

namespace al {
class AddDemoInfo;
struct ActorInitInfo;
class DemoActorHolder;
}  // namespace al

class StageEventDemoBase : public al::LiveActor {
public:
    StageEventDemoBase(const char* name);

    void init(const al::ActorInitInfo& info) override;
    virtual void startDemo() = 0;
    virtual void endDemo() = 0;
    virtual bool isStageStartDemo() const = 0;
    virtual bool isDemoLastStep() const = 0;
    virtual bool isEndDemo() const = 0;
};

static_assert(sizeof(StageEventDemoBase) == 0x108);

class StageEventDemo : public StageEventDemoBase, public IUseDemoSkip {
public:
    StageEventDemo(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void startDemo() override;
    void endDemo() override;
    bool isStageStartDemo() const override;
    bool isDemoLastStep() const override;
    bool isEndDemo() const override;
    bool isFirstDemo() const override;
    bool isEnableSkipDemo() const override;
    void skipDemo() override;
    void updateOnlyDemoGraphics() override;

    void endDemoCore();
    void exeWait();
    void exeDemo();
    void exeEnd();

private:
    al::DemoActorHolder* mDemoActorHolder = nullptr;
    bool mIsStageStartDemo = true;
    SaveObjInfo* mSaveObjInfo = nullptr;
    bool mIsEnableStartDemo = true;
    const char* mDemoStageName = nullptr;
    bool mIsSaveDemo = true;
    bool mIsEnableSkipDemo = false;
    bool mIsDemoLastStep = false;
    al::AddDemoInfo* mAddDemoInfo = nullptr;
};

static_assert(sizeof(StageEventDemo) == 0x148);
