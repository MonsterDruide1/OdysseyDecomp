#include "MapObj/EffectObjAlpha.h"

#include "Library/Effect/EffectSystemInfo.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

EffectObjAlpha::EffectObjAlpha(const char* name) : al::EffectObj(name) {}

void EffectObjAlpha::init(const al::ActorInitInfo& info) {
    using EffectObjAlphaFunctor = al::FunctorV0M<EffectObjAlpha*, void (EffectObjAlpha::*)()>;

    al::EffectObj::init(info);
    al::tryGetArg(&mAlpha, info, "Alpha");
    al::listenStageSwitchOnOff(this, "SwitchAlphaChange",
                               EffectObjAlphaFunctor(this, &EffectObjAlpha::onAlpha),
                               EffectObjAlphaFunctor(this, &EffectObjAlpha::offAlpha));
}

void EffectObjAlpha::onAlpha() {
    al::setEffectParticleAlpha(this, "Wait", mAlpha);
}

void EffectObjAlpha::offAlpha() {}
