#pragma once

#include "Library/Audio/AudioKeeper.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerCostumeInfo.h"

namespace al {
class LiveActor;
class ActorDitherAnimator;
}

class PlayerFunction {
public:
    static u32 getPlayerInputPort(const al::LiveActor*);
    static PlayerCostumeInfo* initMarioModelActor(al::LiveActor *,al::ActorInitInfo const&,char const*,char const*,al::AudioKeeper *,bool);
    static void initMarioModelActor2D(al::LiveActor *,al::ActorInitInfo const&,char const*,bool);
    static bool isInvisibleCap(PlayerCostumeInfo const*);
    static PlayerConst* createMarioConst(char const*);
    static al::ActorDitherAnimator* createPlayerDitherAnimator(al::LiveActor *,float);


};
