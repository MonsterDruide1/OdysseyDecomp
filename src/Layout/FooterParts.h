#pragma once

#include <basis/seadTypes.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}

class FooterParts : public al::LayoutActor {
public:
    FooterParts(al::LayoutActor* parentLayout, const al::LayoutInitInfo& info,
                const char16* footerText, const char* textPane, const char* partPane);

    void changeText(const char16* text);
    void changeTextFade(const char16* text);
    bool tryChangeTextFade(const char16* text);

    void exeWait();
    void exeFadeOut();
    void exeFadeIn();

private:
    const char* mTextPane;
    const char16* mText;
};
