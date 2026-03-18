#pragma once

#include "Library/LiveActor/LiveActor.h"

class BossKnuckleHandPrint : public al::LiveActor {
public:
    BossKnuckleHandPrint(const char* name);
    void initWithArchiveName(const al::ActorInitInfo& initInfo, const char* archiveName);
    void appear() override;
    void startDisappearIfAppeard();
    void exeAppear();
    void exeDisappear();

private:
    bool mIsLeft = false;
};
