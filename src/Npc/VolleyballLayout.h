#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveExecutor.h"

namespace al {
struct ActorInitInfo;
}
class MiniGameCueLayout;
class MiniGameLayout;
class VolleyballNpc;

class VolleyballLayout : public al::NerveExecutor {
public:
    VolleyballLayout(const VolleyballNpc* npc, const al::ActorInitInfo& info);
    void exeWait();
    void exeAppear();
    void exeGame();
    void exeResult();
    void exeEnd();
    void exeMessage();
    bool isWaiting() const;
    bool isInGame() const;
    void start();
    void result();
    void end();
    void setBest(u32 count);
    void setBestToday(u32 count);

private:
    const VolleyballNpc* mNpc = nullptr;
    MiniGameLayout* mLayout = nullptr;
    MiniGameCueLayout* mCueLayout = nullptr;
    bool _28 = false;
};

static_assert(sizeof(VolleyballLayout) == 0x30);
