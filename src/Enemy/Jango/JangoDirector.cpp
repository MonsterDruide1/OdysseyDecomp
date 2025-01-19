#include "Enemy/Jango/JangoDirector.h"

JangoDirector::JangoDirector() {}

const char* JangoDirector::getSceneObjName() const {
    return "ジャンゴ保持者";
}

bool JangoDirector::isEnableCap() const {
    return true;
}

void JangoDirector::placeCap() {
    mIsEnableCap = true;
}

void JangoDirector::registerJango(Jango* jango) {
    mJangos[mJangoCount] = jango;
    ++mJangoCount;
}

void JangoDirector::removeCap() {
    mIsEnableCap = false;
}
