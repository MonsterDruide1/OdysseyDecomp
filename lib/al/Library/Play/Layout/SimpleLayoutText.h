#pragma once

#include <gfx/seadColor.h>
#include <math/seadVector.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
struct ActorInitInfo;
class LayoutInitInfo;

class SimpleLayoutText : public LayoutActor {
public:
    SimpleLayoutText(const LayoutInitInfo& info, const char* name, const char* paneName,
                     const char* archiveName);

    void start(const sead::Vector2f& pos, const char* str, s32 lifetime);
    void setPos(const sead::Vector2f& pos);
    void setText(const char* text);
    void start(const sead::Vector2f& pos, const char16* str, s32 lifetime);
    void setText(const char16* str);
    void start(const sead::Vector2f& pos, const char* category, const char* key, s32 lifetime);
    void setScale(f32 scale);
    void setColor(const sead::Color4u8& color);
    void setPositionCenterH();

    void exeWait();

private:
    s32 mLifetime = -1;
    const char* mPaneName;
};

SimpleLayoutText* createSimpleLayoutText(const ActorInitInfo& info, const char* name,
                                         const char* paneName, const char* archiveName);
}  // namespace al
