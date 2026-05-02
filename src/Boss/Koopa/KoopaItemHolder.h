#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}  // namespace al

class KoopaItemHolder {
public:
    KoopaItemHolder();

    bool tryGenerateLifeUpItem(al::LiveActor* actor);

private:
    al::LiveActor* mLifeUpItem = nullptr;
    s32 mGenerateDelay = 0;
    s32 mGenerateCount = 1;
    bool mIsEnableGenerate = false;
};

static_assert(sizeof(KoopaItemHolder) == 0x18);
