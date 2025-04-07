#include "Scene/ProjectSceneFactory.h"

#include "Library/Scene/CreateSceneFunc.h"

#include "Scene/TitleMenuScene.h"

const al::NameToCreator<alSceneFunction::SceneCreatorFunction> sProjectSceneFactoryEntries[] = {
    {"DemoChangeWorldScene", nullptr},
    {"DemoScene", nullptr},
    {"DemoSceneWithCinemaCaption", nullptr},
    {"EndingScene", nullptr},
    {"FirstSequenceScene", nullptr},
    {"StageScene", nullptr},
    {"StaffRollScene", nullptr},
    {"TitleMenuScene", alSceneFunction::createSceneFunc<TitleMenuScene>},
    {"WorldWarpHoleScene", nullptr}};

ProjectSceneFactory::ProjectSceneFactory() : SceneFactory("シーン生成") {
    initFactory(sProjectSceneFactoryEntries);
}
