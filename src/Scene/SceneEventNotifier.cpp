#include "Scene/SceneEventNotifier.h"

#include "Library/Base/StringUtil.h"
#include "Library/Scene/SceneObjUtil.h"

#include "Scene/SceneEventListenerHolder.h"

SceneEventNotifierInitTable sSceneEventNotifierInitTables[] = {{"SnapShotModeOn", 256},
                                                               {"SnapShotModeOff", 256}};

// NON_MATCHING but equivalent https://decomp.me/scratch/bNYHm
SceneEventNotifier::SceneEventNotifier() {
    initListenerHolderList(sSceneEventNotifierInitTables);
}

void SceneEventNotifier::initListenerHolderList(SceneEventNotifierInitTable* initTables,
                                                s32 initTableCount) {
    mListenerHolderCount = initTableCount;
    mListenerHolders = new SceneEventListenerHolder*[mListenerHolderCount];

    for (s32 i = 0; i < mListenerHolderCount; i++)
        mListenerHolders[i] = new SceneEventListenerHolder(initTables[i].name, initTables[i].count);
}

void SceneEventNotifier::addListener(const char* holderName, SceneEventListener* listener) {
    findSceneEventListenerHolder(holderName)->add(listener);
}

SceneEventListenerHolder* SceneEventNotifier::findSceneEventListenerHolder(const char* holderName) {
    for (s32 i = 0; i < mListenerHolderCount; i++)
        if (al::isEqualString(mListenerHolders[i]->getName(), holderName))
            return mListenerHolders[i];

    return nullptr;
}

void SceneEventNotifier::notify(const char* holderName) {
    findSceneEventListenerHolder(holderName)->notify();
}

SceneEventListenerHolder*
SceneEventNotifier::tryFindSceneEventListenerHolder(const char* holderName) {
    return findSceneEventListenerHolder(holderName);
}

namespace rs {
void notifySceneEvent(const al::IUseSceneObjHolder* user, const char* holderName) {
    al::createSceneObj<SceneEventNotifier>(user)->notify(holderName);
}

void listenSceneEvent(const al::IUseSceneObjHolder* user, const char* holderName,
                      SceneEventListener* listener) {
    al::createSceneObj<SceneEventNotifier>(user)->addListener(holderName, listener);
}
}  // namespace rs
