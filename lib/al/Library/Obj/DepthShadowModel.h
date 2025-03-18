#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class ActorInitInfo;

class DepthShadowModel : public LiveActor {
public:
    DepthShadowModel(LiveActor* parent, const ActorInitInfo& info, const char* executorDrawName,
                     bool isVisible);
    DepthShadowModel(LiveActor* parent, const ActorInitInfo& info, const char* childArchiveName,
                     const char* childSuffix, bool isVisible);
    void initAfterPlacement() override;

private:
    sead::Vector3f mPos = {0, 0, 0};
    bool mIsVisible = true;
};

static_assert(sizeof(DepthShadowModel) == 0x118, "DepthShadowModel Size");
}  // namespace al
