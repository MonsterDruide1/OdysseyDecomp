#pragma once

#include "Library/Obj/PartsModel.h"

class TiaraEyes : public al::PartsModel {
public:
    TiaraEyes(char const* name);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void startWaitPanicL(float frame);
    void startWaitPanicR(float frame);
    void startMovePanicL(float frame);
    void startMovePanicR(float frame);
    void startReaction();
    void startReactionCap();
    void exeAppear();
    void exeWait();
    void exeDisappear();
    void exeHide();
    void endHide();
    void exeControlled();

private:
    bool mIsActive = false;
    s32 mHideTimer = 0;
};
