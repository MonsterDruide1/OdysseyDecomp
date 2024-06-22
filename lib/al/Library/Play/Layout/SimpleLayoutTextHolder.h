#pragma once

#include <gfx/seadColor.h>
#include <math/seadVector.h>

namespace al {
class LayoutInitInfo;
class SimpleLayoutText;

class SimpleLayoutTextWrapper {
public:
    SimpleLayoutTextWrapper(SimpleLayoutText* simpleLayoutText);
    SimpleLayoutTextWrapper* start(const sead::Vector2f& pos, const char* str, s32 lifetime);
    SimpleLayoutTextWrapper* start(const sead::Vector2f& pos, const char16_t* str, s32 lifetime);
    SimpleLayoutTextWrapper* setScale(f32 scale);
    SimpleLayoutTextWrapper* setColor(const sead::Color4u8& color);
    SimpleLayoutTextWrapper* setPos(const sead::Vector2f& pos);

private:
    SimpleLayoutText* mSimpleLayoutText;
};

struct SimpleLayoutTextHolderEntry {
    SimpleLayoutTextHolderEntry(SimpleLayoutText* e) : text(e) {}
    SimpleLayoutText* text;
};

class SimpleLayoutTextHolder {
public:
    SimpleLayoutTextHolder(s32 amount, const LayoutInitInfo& info, const char* name,
                           const char* paneName, const char* archiveName);
    void reset();
    SimpleLayoutTextHolderEntry* start(const sead::Vector2f& pos, const char* str);
    SimpleLayoutTextHolderEntry* start(const sead::Vector2f& pos, const char16_t* str);

private:
    s32 mCurrentIndex;
    s32 mAmount;
    SimpleLayoutText** mLayouts;
    SimpleLayoutTextHolderEntry** mEntries;
};
}  // namespace al
