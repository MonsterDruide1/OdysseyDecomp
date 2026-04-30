#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Scene/ISceneObj.h"

namespace al {
class BalloonOrderGroupHolder;
class EventFlowChoiceInfo;
class EventFlowExecutorHolder;
class EventFlowNode;
class LiveActor;
class SceneEventFlowMsg;
}  // namespace al
class CostumePatternChecker;
class EventFlowSceneExecuteCtrl;
class NpcEventBalloonInfo;
class NpcEventSceneConstData;
class NpcEventTalkInfo;

class NpcEventSceneInfo {
public:
    const al::LiveActor* getBalloonActor() const { return mBalloonActor; }

    bool isPlayingBalloonMessageVoice() const { return mIsPlayingBalloonMessageVoice; }

    bool isOpenWaitTalkMessage() const { return mIsOpenWaitTalkMessage; }

    bool isPlayingTextPaneAnim() const { return !mIsEndTextPaneAnim; }

    bool isTalkMessageClosed() const { return mTalkMessageInfo->isClosed(); }

    bool isCloseWipeFadeBlack() const { return mWipeFadeBlackState == 1; }

    bool isOpenWipeFadeBlack() const { return mWipeFadeBlackState == -1; }

    bool isExistNpcLookPos() const { return mIsExistNpcLookPos; }

    const sead::Vector3f& getNpcLookPos() const { return mNpcLookPos; }

    bool isPlayerInWater() const { return mIsPlayerInWater == 1; }

private:
    struct TalkMessageInfo {
        bool isClosed() const { return message == nullptr; }

        const char16* message = nullptr;
    };

    const al::LiveActor* mBalloonActor = nullptr;
    TalkMessageInfo* mTalkMessageInfo = nullptr;
    bool mIsPlayingBalloonMessageVoice = false;
    bool mIsOpenWaitTalkMessage = false;
    bool _12 = false;
    bool mIsEndTextPaneAnim = false;
    bool mIsExistNpcLookPos = false;
    char _15[0x3] = {};
    s32 mWipeFadeBlackState = 0;
    sead::Vector3f mNpcLookPos = sead::Vector3f::zero;
    char _28[0xc] = {};
    s32 mIsPlayerInWater = 0;
};

class NpcEventMessageStyleInfo {
public:
    virtual void clear();
    virtual void copy(const NpcEventMessageStyleInfo&);
    virtual void _10();
    virtual void update();

    s64 getStyle() const { return mStyle; }

private:
    s64 mStyle = 0;
};

class NpcEventSceneConstData {
public:
    s64 getEventTalkWindowMessageStyle() const {
        NpcEventMessageStyleInfo* info = &mMessageStyleInfo;
        info->update();
        return info->getStyle();
    }

private:
    char _0[0x38] = {};
    mutable NpcEventMessageStyleInfo mMessageStyleInfo;
};

class NpcEventCtrlInfo : public al::ISceneObj {
public:
    NpcEventCtrlInfo(const NpcEventSceneInfo& sceneInfo, const NpcEventSceneConstData& constData,
                     EventFlowSceneExecuteCtrl* sceneExecuteCtrl);

    bool isCloseTalk() const;
    void popBalloonInfo(NpcEventBalloonInfo* info);
    void popTalkInfo(NpcEventTalkInfo* info);
    void requestShowBalloonMessage(const NpcEventBalloonInfo& info);
    void requestShowTalkMessage(const al::EventFlowNode* node, const NpcEventTalkInfo& info);
    void requestCloseTalkMessage(const al::LiveActor* actor);
    void requestCloseWipeFadeBlack(al::EventFlowNode* node, s32 step);
    void requestOpenWipeFadeBlack(al::EventFlowNode* node, s32 step);
    void setBalloonFilterOnlyMiniGame(const al::LiveActor* actor);
    void resetBalloonFilter(const al::LiveActor* actor);
    void startChoice(const al::EventFlowNode* node, al::EventFlowChoiceInfo* choiceInfo);
    s32 getChoiceMessageNum() const;
    const char16* getChoiceMessage(s32 index) const;
    s32 getChoiceCancelIndex() const;
    const char16* tryGetChoiceTalkMessage() const;
    void endChoice(s32 index);
    bool isEnableCancelChoice() const;
    const char* getSceneObjName() const override;

    const al::LiveActor* getCurrentBalloonActor() const { return mBalloonActor; }

    al::SceneEventFlowMsg* getSceneEventFlowMsg() const { return mSceneEventFlowMsg; }

    al::EventFlowExecutorHolder* getEventFlowExecutorHolder() const {
        return mEventFlowExecutorHolder;
    }

    al::BalloonOrderGroupHolder* getBalloonOrderGroupHolder() const {
        return mBalloonOrderGroupHolder;
    }

    CostumePatternChecker* getCostumePatternChecker() const { return mCostumePatternChecker; }

    EventFlowSceneExecuteCtrl* getSceneExecuteCtrl() const { return mSceneExecuteCtrl; }

    bool isSuccessBalloonMessage(const al::LiveActor* actor) const {
        return mSceneInfo->getBalloonActor() == actor;
    }

    bool isPlayingBalloonMessageVoice() const { return mSceneInfo->isPlayingBalloonMessageVoice(); }

    bool isOpenWaitTalkMessage() const { return mSceneInfo->isOpenWaitTalkMessage(); }

    bool isPlayingTextPaneAnim() const { return mSceneInfo->isPlayingTextPaneAnim(); }

    bool isCloseWipeFadeBlack() const { return mSceneInfo->isCloseWipeFadeBlack(); }

    bool isOpenWipeFadeBlack() const { return mSceneInfo->isOpenWipeFadeBlack(); }

    bool isTalkMessageClosed() const { return mSceneInfo->isTalkMessageClosed(); }

    bool isTalkMessageRequestEnd() const { return _90 == nullptr; }

    bool isEndTalkMessage() const { return isTalkMessageClosed() && isTalkMessageRequestEnd(); }

    s64 getEventTalkWindowMessageStyle() const {
        return mSceneConstData->getEventTalkWindowMessageStyle();
    }

    bool isExistNpcLookPos() const { return mSceneInfo->isExistNpcLookPos(); }

    const sead::Vector3f& getNpcLookPos() const { return mSceneInfo->getNpcLookPos(); }

    bool isPlayerInWater() const { return mSceneInfo->isPlayerInWater(); }

private:
    const NpcEventSceneInfo* mSceneInfo;
    const NpcEventSceneConstData* mSceneConstData;
    al::SceneEventFlowMsg* mSceneEventFlowMsg;
    EventFlowSceneExecuteCtrl* mSceneExecuteCtrl;
    al::EventFlowExecutorHolder* mEventFlowExecutorHolder;
    al::BalloonOrderGroupHolder* mBalloonOrderGroupHolder;
    void* _38 = nullptr;
    void* _40 = nullptr;
    s32 _48 = -1;
    void* _50 = nullptr;
    void* _58 = nullptr;
    s32 _60 = 0;
    f32 _64 = 1.0f;
    s32 _68 = 0;
    s32 _6c = -1;
    bool _70 = false;
    char _71[0x7] = {};
    void* _78 = nullptr;
    char16 _80[4] = {};
    const al::LiveActor* mBalloonActor = nullptr;
    void* _90 = nullptr;
    void* _98 = nullptr;
    void* _a0 = nullptr;
    void* _a8 = nullptr;
    s32 _b0 = 0;
    s32 _b4 = -1;
    void* _b8 = nullptr;
    CostumePatternChecker* mCostumePatternChecker = nullptr;
    char16 _c8[2] = {};
    s32 _cc = -1;
    s32 _d0 = -1;
};

static_assert(sizeof(NpcEventCtrlInfo) == 0xd8);
