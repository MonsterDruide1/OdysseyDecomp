#include "Library/Play/Layout/SimpleLayoutAppear.h"
#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutInitInfo.h"

al::SimpleLayoutAppear::SimpleLayoutAppear(const char* a1, const char* a2,
                                           const al::LayoutInitInfo& info, const char* a4)
    : al::LayoutActor(a1) {
    al::initLayoutActor(this, info, a2, a4);
}

void al::SimpleLayoutAppear::appear() {
    al::startAction(this, "Appear", nullptr);
    al::LayoutActor::appear();
}
