#include "al/actor/ProjectAppearSwitchFactory.h"
#include <array>

namespace al {
    
    //FIXME fill in method references: off_7101D89F18
    static FactoryEntry entries[] = {
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
        {"WobbleMapParts", nullptr}
    };

    ProjectAppearSwitchFactory::ProjectAppearSwitchFactory() : ActorFactory("アクター生成") {
        mFactoryEntries = entries;
        mNumFactoryEntries = ssize(entries);
    }

}