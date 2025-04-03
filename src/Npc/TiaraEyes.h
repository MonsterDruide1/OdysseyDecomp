#pragma once

#include "Library/Obj/PartsModel.h"

class TiaraEyes : public al::PartsModel {
public:
    TiaraEyes(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void startWaitPanicL(f32 frame);
    void startWaitPanicR(f32 frame);
    void startMovePanicL(f32 frame);
    void startMovePanicR(f32 frame);
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
