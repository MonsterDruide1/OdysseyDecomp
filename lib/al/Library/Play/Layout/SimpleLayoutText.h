#pragma once

#include <gfx/seadColor.h>
#include <math/seadVector.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class ActorInitInfo;
class LayoutInitInfo;

class SimpleLayoutText : public LayoutActor {
public:
    SimpleLayoutText(const LayoutInitInfo&, const char*, const char*, const char*);

    void start(const sead::Vector2f&, const char*, s32);
    void setPos(const sead::Vector2f&);
    void setText(const char*);
    void start(const sead::Vector2f&, const char16*, s32);
    void setText(const char16*);
    void start(const sead::Vector2f&, const char*, const char*, s32);
    void setScale(f32);
    void setColor(const sead::Color4u8&);
    void setPositionCenterH();

    void exeWait();

private:
    s32 mLifetime = -1;
    const char* mPaneName;
};

SimpleLayoutText* createSimpleLayoutText(const ActorInitInfo&, const char*, const char*,
                                         const char*);
}  // namespace al
