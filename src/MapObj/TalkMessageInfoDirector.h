#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/Execute/IUseExecutor.h"
#include "Library/HostIO/HioNode.h"
#include "Library/Nerve/IUseNerve.h"
#include "Library/Scene/ISceneObj.h"
#include "Library/Scene/IUseSceneObjHolder.h"

namespace al {
struct ActorInitInfo;
class AudioKeeper;
class Nerve;
class NerveKeeper;
class SceneObjHolder;
class SimpleLayoutAppearWaitEnd;
class StageInfo;
}  // namespace al
class CapMessageKeeper;
class TalkMessageInfoDirectorStateAppearMessage;
class TalkMessageInfoDirectorStateAppearWaitEndMessage;
class TalkMessageInfoMessageParam;
class TalkMessageInfoMoon;
class TalkMessageInfoParam;

class TalkMessageInfoDirector : public al::HioNode,
                                public al::IUseNerve,
                                public al::ISceneObj,
                                public al::IUseSceneObjHolder,
                                public al::IUseExecutor,
                                public al::IUseAudioKeeper {
public:
    TalkMessageInfoDirector();

    al::NerveKeeper* getNerveKeeper() const override;
    void execute() override;
    al::SceneObjHolder* getSceneObjHolder() const override;
    al::AudioKeeper* getAudioKeeper() const override;

    void init(const al::ActorInitInfo& info, al::SceneObjHolder* sceneObjHolder);
    s32 findInAreaTalkMessageInfoIdx() const;
    bool isActiveCapMessage(const char* labelName) const;
    bool isDelayCapMessage(const char* labelName) const;
    void endForce();

    void exeWatch();
    void setNerveMessage();
    void exeDelay();
    TalkMessageInfoParam* getInfoParam() const;
    bool changeTalkMessageIdx();
    void exeAppear();
    void exeFollowText();
    void exeWait();
    s32 calcWaitFrame(const TalkMessageInfoMessageParam* messageParam, s32 page) const;
    void exeDisappear();
    void exeEndForce();
    TalkMessageInfoParam* tryGetInfoParam() const;
    bool updateEndForceCommon();

    void exeAppearMessage();
    void exeAppearMessageLow();
    void exeAppearMessageDemo();
    void exeAppearMessageBoss();
    void exeAppearMessageMinigame();
    void exeAppearMessageMoon();
    void exeAppearWaitEndMessage();

    void registerTalkMessageInfoParam(TalkMessageInfoParam* param);
    void tryCreateCapMessageList(const al::StageInfo* stageInfo, const al::ActorInitInfo& info);
    void startPlayCapMessage();
    void endPlayCapMessage();
    void startMessageContinue(const char* labelName);
    void setNerveAppearWaitEndSystemMessage(const char* categoryName, const char* labelName);
    void endMessageContinue();
    bool trySetNerveAppearMessageLow(const char* categoryName, const char* labelName, s32 waitFrame,
                                     s32 delay);
    void setNerveAppearMessageCommon(const char* categoryName, const char* labelName, s32 waitFrame,
                                     s32 delay, const al::Nerve* nerve, bool isStageMessage);
    void trySetNerveAppearMessageMoon();
    void setNerveAppearMessageMoon(const char* categoryName, const char* labelName);
    void setNerveAppearMessage(const char* categoryName, const char* labelName, s32 waitFrame,
                               s32 delay, bool isDemo, bool isStageMessage);
    void setNerveAppearMessageBoss(const char* categoryName, const char* labelName, s32 waitFrame,
                                   s32 delay, bool isStageMessage);
    void setNerveAppearMessageMinigame(const char* categoryName, const char* labelName,
                                       s32 waitFrame, s32 delay);
    void setNerveAppearMessageMoonLowText(const char16* message, const char* labelName);
    void setNerveAppearMessageMoonNum(const char* categoryName, const char* labelName, s32 num);

private:
    TalkMessageInfoParam* mInfoParams[32];
    s32 mInfoParamCount = 0;
    s32 mCurrentInfoParamIdx = -1;
    TalkMessageInfoMoon** mMoonInfos = nullptr;
    s32 mMoonInfoCount = 0;
    al::SimpleLayoutAppearWaitEnd* mLayout = nullptr;
    s32 mCurrentPage = 0;
    TalkMessageInfoDirectorStateAppearMessage* mStateAppearMessage = nullptr;
    void* _158 = nullptr;
    TalkMessageInfoDirectorStateAppearWaitEndMessage* mStateAppearWaitEndMessage = nullptr;
    al::NerveKeeper* mNerveKeeper = nullptr;
    al::SceneObjHolder* mSceneObjHolder = nullptr;
    bool mIsPlayCapMessage = true;
    u8 _179[7];
    CapMessageKeeper* mCapMessageKeeper = nullptr;
    al::AudioKeeper* mAudioKeeper = nullptr;
    sead::WFixedSafeString<256> mMoonNumText;
};

static_assert(sizeof(TalkMessageInfoDirector) == 0x3A8);

namespace TalkMessageFunction {
void registerTalkMessageInfoParam(TalkMessageInfoParam* param,
                                  const al::IUseSceneObjHolder* sceneObjHolder);
TalkMessageInfoDirector* getTalkMessageInfoDirector(const al::IUseSceneObjHolder* sceneObjHolder);
bool isExistTalkMessageInfoDirector(const al::IUseSceneObjHolder* sceneObjHolder);
}  // namespace TalkMessageFunction
