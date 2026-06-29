#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}

class CursorParts : public al::LayoutActor {
public:
    CursorParts(al::LayoutActor* parentLayout, const al::LayoutInitInfo& info, const char* name,
                const char* archiveName);

    void appearStart();
    void end();
    void hide();
    void tryAppearIfHide();
    void exeAppear();
    void exeWait();
    void exeEnd();
    void exeHide();
};
