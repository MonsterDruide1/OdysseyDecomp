#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}

class SimpleLayoutMenu : public al::LayoutActor {
public:
    SimpleLayoutMenu(const char* name, const char* layoutName, const al::LayoutInitInfo& info,
                     const char* archiveName, bool localize);
    SimpleLayoutMenu(al::LayoutActor* parent, const char* name, const char* layoutName,
                     const al::LayoutInitInfo& info, const char* archiveName);

    void startAppear(const char* actionName);
    void startEnd(const char* actionName);
    void exeAppear();
    void exeWait();
    void exeEnd();
    void exeEndWait();
    bool isAppearOrWait() const;
    bool isWait() const;
    bool isEndWait() const;

private:
    s32 mWaitDuration = -1;
};
