#pragma once

#include "Library/Layout/LayoutActor.h"
#include "Library/LiveActor/ActorInitInfo.h"

namespace al {
class ActorInitInfo;
class LayoutInitInfo;

class SimpleLayoutText : public al::LayoutActor {
public:
    SimpleLayoutText(al::LayoutInitInfo const&, const char*, const char*, const char*);

    void start(const sead::Vector2f&, const char*, s32);
    void setPos(const sead::Vector2f&);
    void setText(const char*);
    void start(const sead::Vector2f&, const char16*, s32);
    void setText(const char16*);
    void start(const sead::Vector2f&, const char*, const char*, s32);
    void setScale(float);
    void setColor(sead::Color4u8 const&);
    void setPositionCenterH();

    void exeWait();
};

void createSimpleLayoutText(const al::ActorInitInfo&, const char*, const char*, const char*);
}  // namespace al
