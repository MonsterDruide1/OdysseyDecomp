#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}

class FooterParts : public al::LayoutActor {
public:
    FooterParts(al::LayoutActor*, const al::LayoutInitInfo&, const char16_t*, const char*,
                const char*);

    void changeText(const char16_t*);
    void changeTextFade(const char16_t*);
    bool tryChangeTextFade(const char16_t*);

    void exeWait();
    void exeFadeOut();
    void exeFadeIn();

private:
    const char* mTextPane;
    const char16_t* mText;
};