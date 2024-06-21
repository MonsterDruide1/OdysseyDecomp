#include "Library/Play/Layout/SimpleLayoutAppear.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutInitInfo.h"

al::SimpleLayoutAppear::SimpleLayoutAppear(const char* name, const char* layoutName,
                                           const LayoutInitInfo& info, const char* archiveName)
    : LayoutActor(name) {
    initLayoutActor(this, info, layoutName, archiveName);
}

void al::SimpleLayoutAppear::appear() {
    startAction(this, "Appear", nullptr);
    LayoutActor::appear();
}
