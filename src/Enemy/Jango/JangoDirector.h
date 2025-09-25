#pragma once

#include <basis/seadTypes.h>

#include "Library/Scene/ISceneObj.h"

class Jango;

class JangoDirector : public al::ISceneObj {
public:
    JangoDirector();

    const char* getSceneObjName() const override { return "ジャンゴ保持者"; }

    void registerJango(Jango* jango);
    bool isEnableCap() const;
    void placeCap();
    void removeCap();

private:
    Jango** mJangos = new Jango*[10];
    s32 mJangoCount = 0;
    bool mIsEnableCap = false;
};
