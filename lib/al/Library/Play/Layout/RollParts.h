#pragma once

#include <math/seadVector.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;

class RollParts : public LayoutActor {
public:
    enum class ActionType : u32 { RollRight, RollLeft, Deactive, Active };

    RollParts(LayoutActor* parent, const LayoutInitInfo& info, const char* archiveName);

    void startLoopAction(const char* actionName, const char* paneName);
    void setData(const char16** messages, s32 messageCount, bool isLoop, s32 selectedIdx,
                 const char* paneName);
    void updateHeaderText();
    void setSelectedIdx(s32 idx);
    void activate();
    void activate(s32 selectedIdx);
    void deactivate();
    void rollRight();
    void rollLeft();

    void calcCursorTrans(sead::Vector2f* outCursorTrans) const;
    bool isJustChangeRoll() const;
    bool isRoll() const;

    void exeDeactive();
    void exeActive();
    void exeRollOut();
    void exeRollIn();

private:
    const char16** mMessages = nullptr;
    s32 mMessageCount = -1;
    s32 mSelectedIdx = -1;
    bool mIsLoop = false;
    const char* mPaneName = "TxtRoll";
    const char* mHeaderPaneName = nullptr;
    const char* mRollPaneName = "Change";
    const char* mActiveAction = "Select";
    const char* mDeactiveAction = "Wait";
    const char* mRollRightInAction = "RightIn";
    const char* mRollRightOutAction = "RightOut";
    const char* mRollLeftInAction = "LeftIn";
    const char* mRollLeftOutAction = "LeftOut";
    ActionType mCurrentActionType = ActionType::Active;
    ActionType mNextActionType = ActionType::Active;
    bool mHasStatePane = false;
};

static_assert(sizeof(RollParts) == 0x1a0);
}  // namespace al
