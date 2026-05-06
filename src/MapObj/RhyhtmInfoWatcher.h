#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
struct ActorInitInfo;
class IUseSceneObjHolder;
}  // namespace al

class RhyhtmInfoWatcher : public al::LiveActor, public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_RhyhtmInfoWatcher;

    RhyhtmInfoWatcher(const char* name);

    void initAfterPlacementSceneObj(const al::ActorInitInfo& info) override;
    void control() override;

    virtual f32 getCurrentBeat() const { return mCurrentBeat; }

    virtual f32 getBeatDelta() const { return mCurrentBeat - mPreviousBeat; }

    virtual bool isLooping() const { return mIsLooping; }

private:
    bool loopCheck();

    f32 mCurrentBeat = -1.0f;
    f32 mPreviousBeat = -1.0f;
    bool mIsLooping = false;
};

static_assert(sizeof(RhyhtmInfoWatcher) == 0x120);

namespace rs {
RhyhtmInfoWatcher* getInstance(al::IUseSceneObjHolder* holder);
void registerRhyhtmInfoListener(al::IUseSceneObjHolder* holder);
f32 getCurrentBeat(al::IUseSceneObjHolder* holder);
f32 getBeatDelta(al::IUseSceneObjHolder* holder);
bool isLooping(al::IUseSceneObjHolder* holder);
}  // namespace rs
