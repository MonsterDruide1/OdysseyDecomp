#pragma once

#include <math/seadVector.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;

class RollParts : public LayoutActor {
public:
    RollParts(LayoutActor* parent, const LayoutInitInfo& info, const char* archiveName);

    void startLoopAction(const char* actionName, const char* paneName);
    void setData(const char16**, s32, bool, s32, const char*);
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
    s32 _138 = -1;
    s32 mSelectedIdx = -1;
    bool _140 = false;
    const char* mPaneName = "TxtRoll";
    const char* _150 = nullptr;
    const char* _158 = "Change";
    const char* mActiveAction = "Select";
    const char* mDeactiveAction = "Wait";
    const char* _170 = "RightIn";
    const char* _178 = "RightOut";
    const char* _180 = "LeftIn";
    const char* _188 = "LeftOut";
    u32 _190 = 3;
    u32 _194 = 3;
    bool _198 = false;
};

static_assert(sizeof(RollParts) == 0x1a0);
}  // namespace al
