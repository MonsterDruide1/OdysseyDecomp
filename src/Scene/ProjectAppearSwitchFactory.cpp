#include "Scene/ProjectAppearSwitchFactory.h"

// FIXME fill in method references: (1.0) off_7101D89F18
static al::NameToCreator<al::ActorCreatorFunction> sProjectAppearSwitchFactoryEntries[] = {
    {"FixMapParts", nullptr},
    {"FallMapParts", nullptr},
    {"CapHanger", nullptr},
    {"Coin", nullptr},
    {"CoinCollect", nullptr},
    {"FixMapPartsCapHanger", nullptr},
    {"FloaterMapParts", nullptr},
    {"KeyMoveMapParts", nullptr},
    {"MeganeMapParts", nullptr},
    {"PoleGrabCeil", nullptr},
    {"RailDrawer", nullptr},
    {"ReactionObject", nullptr},
    {"RotateMapParts", nullptr},
    {"SeesawMapParts", nullptr},
    {"Tank", nullptr},
    {"TreasureBox", nullptr},
    {"TreasureBoxKey", nullptr},
    {"WaveSurfMapParts", nullptr},
    {"WobbleMapParts", nullptr}};

ProjectAppearSwitchFactory::ProjectAppearSwitchFactory() : ActorFactory("アクター生成") {
    initFactory(sProjectAppearSwitchFactoryEntries);
}
