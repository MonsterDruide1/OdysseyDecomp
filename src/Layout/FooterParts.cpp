#include "Layout/FooterParts.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(FooterParts, Wait);
NERVE_IMPL(FooterParts, FadeOut);
NERVE_IMPL(FooterParts, FadeIn);

NERVES_MAKE_NOSTRUCT(FooterParts, Wait, FadeOut, FadeIn);
}  // namespace

FooterParts::FooterParts(al::LayoutActor* parentLayout, const al::LayoutInitInfo& info,
                         const char16_t* footerText, const char* textPane, const char* partPane)
    : al::LayoutActor("フッターパーツ"), mTextPane(textPane), mText(footerText) {
    al::initLayoutPartsActor(this, parentLayout, info, partPane, nullptr);

    al::setPaneString(this, mTextPane, mText, 0);
    initNerve(&Wait, 0);
}

void FooterParts::changeText(const char16_t* text) {
    mText = text;
    al::setPaneString(this, mTextPane, text, 0);
}

void FooterParts::changeTextFade(const char16_t* text) {
    mText = text;
    al::setNerve(this, &FadeIn);
}

bool FooterParts::tryChangeTextFade(const char16_t* text) {
    if (mText == text)
        return false;
    changeTextFade(text);
    return true;
}

void FooterParts::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait", 0);
}

void FooterParts::exeFadeOut() {}

void FooterParts::exeFadeIn() {
    if (al::isFirstStep(this)) {
        al::setPaneString(this, mTextPane, mText, 0);
        al::startAction(this, "FadeIn", 0);
    }
    if (al::isActionEnd(this, nullptr))
        al::setNerve(this, &Wait);
}
