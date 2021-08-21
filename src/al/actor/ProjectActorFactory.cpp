#include "al/actor/ProjectActorFactory.h"

namespace al {

    //FIXME VER_100 only
    static FactoryEntry entries[0x23A] = {

    };

    ProjectActorFactory::ProjectActorFactory() : ActorFactory("アクター生成") {//("繧｢繧ｯ繧ｿ繝ｼ逕滓")
        mFactoryEntries = entries;
        #ifdef VER_100
        mNumFactoryEntries = 0x23A;
        #endif

        #ifdef VER_110
        mNumFactoryEntries = 0x23A;
        #endif

        #ifdef VER_120
        mNumFactoryEntries = 0x23B;
        #endif
    }

}