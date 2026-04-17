#include "MapObj/RhyhtmInfoWatcher.h"

#include "Library/Bgm/BgmLineFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/Scene/SceneObjUtil.h"

RhyhtmInfoWatcher::RhyhtmInfoWatcher(const char* name) : al::LiveActor(name) {}

void RhyhtmInfoWatcher::initAfterPlacementSceneObj(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initExecutorWatchObj(this, info);
    al::initActorBgmKeeper(this, info, nullptr);
    getSceneObjName();
    makeActorAlive();
}

void RhyhtmInfoWatcher::control() {
    const al::IUseAudioKeeper* audioKeeper = this;

    if (!al::isEnableRhythmAnim(audioKeeper, nullptr))
        return;

    mPreviousBeat = mCurrentBeat;

    f32 currentBeat = al::getCurBeat(audioKeeper);
    bool isLooping = !(mPreviousBeat <= currentBeat);

    mCurrentBeat = currentBeat;
    mIsLooping = isLooping;
}

bool RhyhtmInfoWatcher::loopCheck() {
    return !(mPreviousBeat <= mCurrentBeat);
}

f32 RhyhtmInfoWatcher::getCurrentBeat() const {
    return mCurrentBeat;
}

f32 RhyhtmInfoWatcher::getBeatDelta() const {
    return mCurrentBeat - mPreviousBeat;
}

bool RhyhtmInfoWatcher::isLooping() const {
    return mIsLooping;
}

namespace rs {

RhyhtmInfoWatcher* getInstance(al::IUseSceneObjHolder* holder) {
    return al::getSceneObj<RhyhtmInfoWatcher>(holder);
}

void registerRhyhtmInfoListener(al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_RhyhtmInfoWatcher))
        return;

    al::createSceneObj(holder, SceneObjID_RhyhtmInfoWatcher);
}

f32 getCurrentBeat(al::IUseSceneObjHolder* holder) {
    return al::getSceneObj<RhyhtmInfoWatcher>(holder)->getCurrentBeat();
}

f32 getBeatDelta(al::IUseSceneObjHolder* holder) {
    return al::getSceneObj<RhyhtmInfoWatcher>(holder)->getBeatDelta();
}

bool isLooping(al::IUseSceneObjHolder* holder) {
    return al::getSceneObj<RhyhtmInfoWatcher>(holder)->isLooping();
}

}  // namespace rs
