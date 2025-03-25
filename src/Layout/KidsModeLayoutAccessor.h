#pragma once

#include "Library/HostIO/HioNode.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
class IUseSceneObjHolder;
}

class KidsModeLayoutAccessor : public al::HioNode, public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_KidsModeLayoutAccessor;

    KidsModeLayoutAccessor();

    const char* getSceneObjName() const override;

    void setKidsModeLayoutDisable() { mIsKidsModeLayoutDisable = true; }

    void setKidsModeLayoutEnable() { mIsKidsModeLayoutDisable = false; }

    bool isKidsModeLayoutDisable() const { return mIsKidsModeLayoutDisable; }

private:
    bool mIsKidsModeLayoutDisable = false;
};

namespace rs {
void setKidsModeLayoutDisable(const al::IUseSceneObjHolder* user);
void setKidsModeLayoutEnable(const al::IUseSceneObjHolder* user);
bool isKidsModeLayoutDisable(const al::IUseSceneObjHolder* user);
}  // namespace rs
