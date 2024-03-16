#include "al/sequence/Sequence.h"
#include "al/HakoniwaSequence/HakoniwaSequence.h"
#include "al/iuse/IUseSceneCreator.h"

#include "al/audio/AudioDirector.h"
#include "al/scene/Scene.h"

namespace al {

IUseSceneCreator::~IUseSceneCreator() {
    ;
}

/*Sequence::Sequence(const char* name) : NerveExecutor(name), mName(name) {}

Sequence::~Sequence() {
    if (mAudioDirector)
        mAudioDirector->finalize();
}

AudioKeeper* Sequence::getAudioKeeper() const {
    return mAudioKeeper;
}
SceneCreator* Sequence::getSceneCreator() const {
    return mSceneCreator;
}
void Sequence::setSceneCreator(SceneCreator* creator) {
    mSceneCreator = creator;
}

void Sequence::init(const al::SequenceInitInfo& initInfo) {}

void Sequence::update() {
    if (mNextScene) {
        mCurrentScene = mNextScene;
        mNextScene = nullptr;
    }

    if (mCurrentScene && mCurrentScene->getIsAlive())
        mCurrentScene->movement();
    if (mAudioDirector)
        mAudioDirector->updatePre();

    updateNerve();

    if (mAudioDirector)
        mAudioDirector->updatePost();
}

void Sequence::kill() {
    isAlive = false;
}

void Sequence::drawMain() const {
    if(!mCurrentScene || !mCurrentScene->getIsAlive())
        return;
    
    mCurrentScene->drawMain();
}

void Sequence::drawSub() const {
    if(!mCurrentScene || !mCurrentScene->getIsAlive())
        return;
    
    mCurrentScene->drawSub();
}

bool Sequence::isDisposable() const {
    return true;
}

Scene* Sequence::getCurrentScene() const {
    return nullptr;
}

/*
    void initAudio(const GameSystemInfo& info, const char*, int, int, int, const char*);
    void initAudioKeeper(const char*);
    void initDrawSystemInfo(const SequenceInitInfo& info);
    const AudioSystemInfo& getAudioSystemInfo() const;
*/

}  // namespace al
