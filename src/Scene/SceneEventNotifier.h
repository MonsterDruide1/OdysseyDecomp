#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
class IUseSceneObjHolder;
}  // namespace al

struct SceneEventListener;
class SceneEventListenerHolder;

struct SceneEventNotifierInitTable {
    const char* name;
    s32 count;
};

class SceneEventNotifier : public al::ISceneObj, public al::HioNode {
public:
    static constexpr s32 sSceneObjId = SceneObjID_SceneEventNotifier;

    SceneEventNotifier();

    template <s32 N>
    inline void initListenerHolderList(SceneEventNotifierInitTable (&initTables)[N]) {
        initListenerHolderList(initTables, N);
    }

    void addListener(const char* holderName, SceneEventListener* listener);
    SceneEventListenerHolder* findSceneEventListenerHolder(const char* holderName);
    void notify(const char* holderName);
    SceneEventListenerHolder* tryFindSceneEventListenerHolder(const char* holderName);

private:
    void initListenerHolderList(SceneEventNotifierInitTable* initTables, s32 initTableCount);

    SceneEventListenerHolder** mListenerHolders;
    s32 mListenerHolderCount;
};

namespace rs {
void notifySceneEvent(const al::IUseSceneObjHolder* user, const char* holderName);
void listenSceneEvent(const al::IUseSceneObjHolder* user, const char* holderName,
                      SceneEventListener* listener);
}  // namespace rs
