#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Demo/IUseDemoSkip.h"

namespace al {
class CameraPoser;
class CameraTicket;
}  // namespace al

namespace sead {
template <typename T>
class PtrArray;
}  // namespace sead

class QuestInfo;

class ScenarioStartCamera : public al::LiveActor, public IUseDemoSkip {
public:
    ScenarioStartCamera(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void appear() override;
    void kill() override;
    const sead::Matrix34f* getBaseMtx() const override;

    bool isEnableStart() const;
    bool isFirstDemo() const override;
    bool isEnableSkipDemo() const override;
    void skipDemo() override;

    void end();
    void exePlay();
    al::CameraPoser* getPoser(s32 index) const;
    void exePlayAnim();
    void exeEnd();
    s32 getMaxPlayStep();

private:
    // Note: the original member names are unverified; member names do not affect codegen.
    s32 mCurrentIndex = 0;                                  // 0x110: index into mTickets (see end())
    s32 mTicketNum = 0;                                     // 0x114: ticket array size, set in init()
    al::CameraTicket** mTickets = nullptr;                  // 0x118
    al::CameraTicket* mDemoCameraTicket = nullptr;          // 0x120: from al::initDemoAnimCamera
    const char* mDemoAnimName = nullptr;                    // 0x128: from al::getStringArg
    const sead::Matrix34f* mBaseMtx = nullptr;              // 0x130
    s32 mLinkedShineIndex = -1;                             // 0x138: from GameDataFunction::tryFindLinkedShineIndex
    sead::PtrArray<QuestInfo>* mQuestInfos = nullptr;       // 0x140: from rs::tryCreateAndRegisterQuestInfoToHolderFromLinkedObj
    bool mIsFirstQuest = false;                             // 0x148: set in initAfterPlacement()
};

static_assert(sizeof(ScenarioStartCamera) == 0x150);
