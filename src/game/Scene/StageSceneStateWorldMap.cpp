#include "game/Scene/StageSceneStateWorldMap.h"

StageSceneStateWorldMap* StageSceneStateWorldMap::tryCreate(al::Scene*, const al::SceneInitInfo&,
                                                            const al::ActorInitInfo&,
                                                            const al::LayoutInitInfo&,
                                                            StageSceneStateCollectionList*) {
    return nullptr;
}

StageSceneStateWorldMap::StageSceneStateWorldMap(const char* name, al::Scene* host,
                                                 ShineTowerRocket* shineTowerRocket,
                                                 const al::SceneInitInfo& sceneInitInfo,
                                                 const al::ActorInitInfo& actorInitInfo,
                                                 const al::LayoutInitInfo& lytInitInfo,
                                                 StageSceneStateCollectionList* stateCollectList)
    : al::HostStateBase<al::Scene>(name, host) {}

StageSceneStateWorldMap::~StageSceneStateWorldMap() {}

void StageSceneStateWorldMap::control() {}
void StageSceneStateWorldMap::appear() {}
void StageSceneStateWorldMap::kill() {}

void StageSceneStateWorldMap::exeInit() {}
void StageSceneStateWorldMap::exeDemoPrep() {}
void StageSceneStateWorldMap::exeDemoStart() {}
void StageSceneStateWorldMap::appearWorldSelectLayout() {}
void StageSceneStateWorldMap::exeDemoWorldOpen() {}
void StageSceneStateWorldMap::exeDemoWorldUnlock() {}
void StageSceneStateWorldMap::exeDemoWorldSelect() {}
void StageSceneStateWorldMap::exeDemoWorldComment() {}
void StageSceneStateWorldMap::exeWait() {}
void StageSceneStateWorldMap::exeIgnoreMessage() {}
void StageSceneStateWorldMap::exeCollectionListMain() {}
void StageSceneStateWorldMap::exeCollectionListExit() {}
void StageSceneStateWorldMap::exeExit() {}
void StageSceneStateWorldMap::exeEnd() {}

void StageSceneStateWorldMap::tryOpenWorldMap() {}
void StageSceneStateWorldMap::isCloseEndWipe() {}
void StageSceneStateWorldMap::isOpenEndWipe() {}
void StageSceneStateWorldMap::isOpenCollectionList() {}
