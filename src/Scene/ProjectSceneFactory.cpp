#include "Scene/ProjectSceneFactory.h"

#include "Library/Factory/Factory.h"
// TODO remove pragma once it is used
#include "Library/Scene/CreateSceneFunc.h"  // IWYU pragma: keep

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
