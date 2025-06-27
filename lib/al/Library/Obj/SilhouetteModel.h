#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {

class SilhouetteModel : public LiveActor {
public:
    SilhouetteModel(LiveActor* parent, const ActorInitInfo& initInfo, const char* category);
    void movement() override;
};

}  // namespace al
