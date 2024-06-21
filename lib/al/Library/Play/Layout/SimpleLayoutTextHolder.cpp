#include "Library/Play/Layout/SimpleLayoutTextHolder.h"

#include "Library/Play/Layout/SimpleLayoutText.h"

al::SimpleLayoutTextWrapper::SimpleLayoutTextWrapper(SimpleLayoutText* simpleLayoutText) {
    mSimpleLayoutText = simpleLayoutText;
}

al::SimpleLayoutTextWrapper* al::SimpleLayoutTextWrapper::start(const sead::Vector2f& pos,
                                                                const char* str, s32 lifetime) {
    mSimpleLayoutText->start(pos, str, lifetime);
    return this;
}

al::SimpleLayoutTextWrapper* al::SimpleLayoutTextWrapper::start(const sead::Vector2f& pos,
                                                                const char16_t* str, s32 lifetime) {
    mSimpleLayoutText->start(pos, str, lifetime);
    return this;
}

al::SimpleLayoutTextWrapper* al::SimpleLayoutTextWrapper::setScale(f32 scale) {
    mSimpleLayoutText->setScale(scale);
    return this;
}

al::SimpleLayoutTextWrapper* al::SimpleLayoutTextWrapper::setColor(const sead::Color4u8& color) {
    mSimpleLayoutText->setColor(color);
    return this;
}

al::SimpleLayoutTextWrapper* al::SimpleLayoutTextWrapper::setPos(const sead::Vector2f& pos) {
    mSimpleLayoutText->setPos(pos);
    return this;
}

al::SimpleLayoutTextHolder::SimpleLayoutTextHolder(s32 amount, const LayoutInitInfo& info,
                                                   const char* name, const char* paneName,
                                                   const char* archiveName)
    : mCurrentIndex(0), mAmount(amount) {
    field_10 = new SimpleLayoutText*[amount];
    field_18 = new SimpleLayoutTextHolderEntry*[amount];

    for (s32 i = 0; i < mAmount; i++) {
        SimpleLayoutText* text = new SimpleLayoutText(info, name, paneName, archiveName);
        field_10[i] = text;
        field_18[i] = new SimpleLayoutTextHolderEntry(field_10[i]);
    }
}

void al::SimpleLayoutTextHolder::reset() {
    mCurrentIndex = 0;
}

al::SimpleLayoutTextHolderEntry* al::SimpleLayoutTextHolder::start(const sead::Vector2f& pos,
                                                                   const char* str) {
    SimpleLayoutTextHolderEntry* entry = field_18[mCurrentIndex++];
    entry->text->start(pos, str, 1);
    return entry;
}

al::SimpleLayoutTextHolderEntry* al::SimpleLayoutTextHolder::start(const sead::Vector2f& pos,
                                                                   const char16_t* str) {
    SimpleLayoutTextHolderEntry* entry = field_18[mCurrentIndex++];
    entry->text->start(pos, str, 1);
    return entry;
}
