//
// Created by cookieso on 10/8/24.
//

#include "JangoDirector.h"

JangoDirector::JangoDirector() : mJangos(new Jango*[10]), mJangoCount(0), mIsEnableCap(false) {}

const char* JangoDirector::getSceneObjName() const {
    return "ジャンゴ保持者";
}

bool JangoDirector::isEnableCap() const {
    return true;
}

void JangoDirector::placeCap() {
    this->mIsEnableCap = true;
}

void JangoDirector::registerJango(Jango* jango) {
    this->mJangos[mJangoCount] = jango;
    ++mJangoCount;
}

void JangoDirector::removeCap() {
    this->mIsEnableCap = false;
}

JangoDirector::~JangoDirector() {}
