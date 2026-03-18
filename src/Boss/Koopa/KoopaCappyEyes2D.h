#pragma once

#include "Library/Obj/PartsModel.h"

class KoopaCappyEyes2D : public al::PartsModel {
public:
    KoopaCappyEyes2D(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;

    void disappear();
    bool isPlayingAppearAnim() const;

    void exeAppear();
    void exeWait();
    void exeDisappear();
};

static_assert(sizeof(KoopaCappyEyes2D) == 0x148);
