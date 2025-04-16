#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;

class SilhouetteModel : public LiveActor {
public:
    SilhouetteModel(LiveActor*, const ActorInitInfo&, const char*);
    void movement() override;
};

}  // namespace al
