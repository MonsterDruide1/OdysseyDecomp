#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveExecutor.h"
#include "Library/Scene/IUseSceneObjHolder.h"

namespace al {
class SceneObjHolder;
}  // namespace al

class KoopaBombHintRequestInfo {
public:
    KoopaBombHintRequestInfo();

private:
    bool mIsRequested = false;
};

static_assert(sizeof(KoopaBombHintRequestInfo) == 0x1);

class KoopaHintHolder : public al::NerveExecutor, public al::IUseSceneObjHolder {
public:
    KoopaHintHolder(al::SceneObjHolder* sceneObjHolder);

    void update();
    bool tryAppearHintCapReflect();
    bool tryAppearMessageDamage(s32 damageType);
    void exeWait();
    void exeHintCapReflect();
    void exeHintCapAttachBomb();
    void exeMessageDamage();

    al::SceneObjHolder* getSceneObjHolder() const override;

private:
    al::SceneObjHolder* mSceneObjHolder = nullptr;
    bool mIsHintCapAttachBombRequested = false;
    bool mIsHintCapReflectEnabled = true;
    s32 mBombHeadHintIndex = -1;
};

static_assert(sizeof(KoopaHintHolder) == 0x28);
