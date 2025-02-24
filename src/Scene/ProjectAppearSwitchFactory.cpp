#include "Scene/ProjectAppearSwitchFactory.h"

#include "Library/LiveActor/CreateActorFunction.h"
#include "Library/MapObj/FallMapParts.h"
#include "Library/MapObj/FixMapParts.h"
#include "Library/MapObj/FloaterMapParts.h"
#include "Library/MapObj/KeyMoveMapParts.h"
#include "Library/MapObj/RotateMapParts.h"
#include "Library/MapObj/SeesawMapParts.h"
#include "Library/MapObj/WobbleMapParts.h"

// FIXME fill in method references: (1.0) off_7101D89F18
const al::NameToCreator<al::ActorCreatorFunction> sProjectAppearSwitchFactoryEntries[] = {
    {"FixMapParts", al::createActorFunction<al::FixMapParts>},
    {"FallMapParts", al::createActorFunction<al::FallMapParts>},
    {"CapHanger", nullptr},
    {"Coin", nullptr},
    {"CoinCollect", nullptr},
    {"FixMapPartsCapHanger", nullptr},
    {"FloaterMapParts", al::createActorFunction<al::FloaterMapParts>},
    {"KeyMoveMapParts", al::createActorFunction<al::KeyMoveMapParts>},
    {"MeganeMapParts", nullptr},
    {"PoleGrabCeil", nullptr},
    {"RailDrawer", nullptr},
    {"ReactionObject", nullptr},
    {"RotateMapParts", al::createActorFunction<al::RotateMapParts>},
    {"SeesawMapParts", al::createActorFunction<al::SeesawMapParts>},
    {"Tank", nullptr},
    {"TreasureBox", nullptr},
    {"TreasureBoxKey", nullptr},
    {"WaveSurfMapParts", nullptr},
    {"WobbleMapParts", al::createActorFunction<al::WobbleMapParts>}};

ProjectAppearSwitchFactory::ProjectAppearSwitchFactory() : ActorFactory("アクター生成") {
    initFactory(sProjectAppearSwitchFactoryEntries);
}
