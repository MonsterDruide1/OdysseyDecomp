#pragma once

#include <basis/seadTypes.h>

#include "Library/Scene/ISceneObj.h"

class Jango;

class JangoDirector : public al::ISceneObj {
public:
    JangoDirector();
    const char* getSceneObjName() const override;
    bool isEnableCap() const;
    void placeCap();
    void registerJango(Jango* jango);
    void removeCap();
    ~JangoDirector() override;

private:
    Jango** mJangos;
    s32 mJangoCount;
    bool mIsEnableCap;
};
