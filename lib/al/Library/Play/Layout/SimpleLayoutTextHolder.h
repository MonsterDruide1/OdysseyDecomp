#pragma once

#include <gfx/seadColor.h>
#include <math/seadVector.h>

namespace al {
class LayoutInitInfo;
class SimpleLayoutText;

class SimpleLayoutTextWrapper {
public:
    SimpleLayoutTextWrapper(SimpleLayoutText*);
    SimpleLayoutTextWrapper* start(const sead::Vector2f&, const char*, s32);
    SimpleLayoutTextWrapper* start(const sead::Vector2f&, const char16_t*, s32);
    SimpleLayoutTextWrapper* setScale(f32);
    SimpleLayoutTextWrapper* setColor(const sead::Color4u8&);
    SimpleLayoutTextWrapper* setPos(const sead::Vector2f&);

private:
    SimpleLayoutText* mSimpleLayoutText;
};

struct SimpleLayoutTextHolderEntry {
    SimpleLayoutTextHolderEntry(SimpleLayoutText* e) : text(e) {}
    SimpleLayoutText* text;
};

class SimpleLayoutTextHolder {
public:
    SimpleLayoutTextHolder(s32, const LayoutInitInfo&, const char*, const char*, const char*);
    void reset();
    SimpleLayoutTextHolderEntry* start(const sead::Vector2f&, const char*);
    SimpleLayoutTextHolderEntry* start(const sead::Vector2f&, const char16_t*);

private:
    s32 mCurrentIndex;
    s32 mAmount;
    SimpleLayoutText** field_10;
    SimpleLayoutTextHolderEntry** field_18;
};
}  // namespace al
