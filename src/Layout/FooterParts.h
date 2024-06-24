#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}

class FooterParts : public al::LayoutActor {
public:
    FooterParts(al::LayoutActor*, const al::LayoutInitInfo&, const char16*, const char*,
                const char*);

    void changeText(const char16*);
    void changeTextFade(const char16*);
    bool tryChangeTextFade(const char16*);

    void exeWait();
    void exeFadeOut();
    void exeFadeIn();

private:
    const char* mTextPane;
    const char16* mText;
};
