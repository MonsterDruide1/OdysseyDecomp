#pragma once

#include <basis/seadTypes.h>

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}

class PlayerConst;

// note: This is just to make the code easier to read. The names are based on PlayerConst function
// names
enum class DeathType : u32 { Damage, Abyss, AbyssWithCapMsg, Press, SandSink, NoOxygen, IceWater };

class PlayerJudgeDeadWipeStart : public IJudge {
public:
    PlayerJudgeDeadWipeStart(const al::LiveActor* playerActor, const PlayerConst* playerConst);
    void reset() override;
    void update() override;
    bool judge() const override;

    bool isDeadTypeDrawForward() const;
    bool isDeadEnableCoinAppear() const;
    void getWipeInfo(const char** name, s32* wait) const;

private:
    const al::LiveActor* mPlayerActor;
    const PlayerConst* mPlayerConst;
    DeathType mDeathType = DeathType::Damage;
    s32 mCounter = -1;
};
