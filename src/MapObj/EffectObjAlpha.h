#pragma once

#include "Library/Obj/EffectObj.h"

class EffectObjAlpha : public al::EffectObj {
public:
    EffectObjAlpha(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void onAlpha();
    void offAlpha();

private:
    f32 mAlpha = 1.0f;
};

static_assert(sizeof(EffectObjAlpha) == 0x160);
