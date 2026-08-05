#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
struct ActorInitInfo;
class LiveActor;
}  // namespace al

class LifeUpItemGenerateWatcher : public al::LiveActor, public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_LifeUpItemGenerateWatcher;

    LifeUpItemGenerateWatcher(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void control() override;
    const char* getSceneObjName() const override;

    static void tryCreate(const al::LiveActor* actor, const al::ActorInitInfo& info);
    bool tryGenerateLifeUpItem(const al::LiveActor* actor);

private:
    al::LiveActor* mGeneratedItem = nullptr;
    s32 mGenerateInterval = 0;
};

static_assert(sizeof(LifeUpItemGenerateWatcher) == 0x120);

namespace rs {
void tryCreateLifeUpItemGenerateWatcher(const al::LiveActor* actor, const al::ActorInitInfo& info);
bool tryGenerateLifeUpItem(const al::LiveActor* actor);
}  // namespace rs
