#include "Scene/ProjectSceneFactory.h"

#include "Library/Scene/CreateSceneFunc.h"

#include "Scene/DemoChangeWorldScene.h"
#include "Scene/DemoScene.h"
#include "Scene/DemoSceneWithCinemaCaption.h"
#include "Scene/EndingScene.h"
#include "Scene/FirstSequenceScene.h"
#include "Scene/StaffRollScene.h"
#include "Scene/StageScene.h"
#include "Scene/TitleMenuScene.h"
#include "Scene/WorldWarpHoleScene.h"

const al::NameToCreator<alSceneFunction::SceneCreatorFunction> sProjectSceneFactoryEntries[] = {
    {"DemoChangeWorldScene", alSceneFunction::createSceneFunc<DemoChangeWorldScene>},
    {"DemoScene", alSceneFunction::createSceneFunc<DemoScene>},
    {"DemoSceneWithCinemaCaption", alSceneFunction::createSceneFunc<DemoSceneWithCinemaCaption>},
    {"EndingScene", alSceneFunction::createSceneFunc<EndingScene>},
    {"FirstSequenceScene", alSceneFunction::createSceneFunc<FirstSequenceScene>},
    {"StageScene", alSceneFunction::createSceneFunc<StageScene>},
    {"StaffRollScene", alSceneFunction::createSceneFunc<StaffRollScene>},
    {"TitleMenuScene", alSceneFunction::createSceneFunc<TitleMenuScene>},
    {"WorldWarpHoleScene", alSceneFunction::createSceneFunc<WorldWarpHoleScene>}};

ProjectSceneFactory::ProjectSceneFactory() : SceneFactory("シーン生成") {
    initFactory(sProjectSceneFactoryEntries);
}
