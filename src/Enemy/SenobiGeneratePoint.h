#pragma once

#include "Library/LiveActor/LiveActor.h"

class Senobi;

class SenobiGeneratePoint : public al::LiveActor {
public:
    using LiveActor::LiveActor;

    void init(const al::ActorInitInfo& initInfo) override;
    void forceKill();
    bool tryGenerate();

    void exeStandby();
    void exeWait();
    void exeEnd();
    void exeGenerate();

    Senobi* getSenobi() const { return mSenobi; }

private:
    Senobi* mSenobi = nullptr;
};
