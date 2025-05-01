#include "Enemy/PackunPoisonBall.h"

#include "Library/Math/ParabolicPath.h"

PackunPoisonBall::PackunPoisonBall(al::LiveActor* param_1, bool isBig)
    : LiveActor("ポイズンパックンの毒だま"), _108(param_1), mIsBig(isBig),
      _118(new al::ParabolicPath()) {
    getName();
}
