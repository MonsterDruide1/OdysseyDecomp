#pragma once

#include "Library/HostIO/HioNode.h"
#include "Library/Scene/ISceneObj.h"

namespace al {
class IUseSceneObjHolder;
}

class KidsModeLayoutAccessor : public al::HioNode, public al::ISceneObj {
public:
    KidsModeLayoutAccessor();
    virtual const char* getSceneObjName() const override;

    bool mIsKidsModeLayoutDisable = false;
};

namespace rs {
void setKidsModeLayoutDisable(const al::IUseSceneObjHolder* user);
void setKidsModeLayoutEnable(const al::IUseSceneObjHolder* user);
bool isKidsModeLayoutDisable(const al::IUseSceneObjHolder* user);
}  // namespace rs
