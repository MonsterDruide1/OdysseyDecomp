#pragma once

#include "al/Library/LiveActor/ActorFactory.h"

namespace al {

class ProjectAppearSwitchFactory : public al::ActorFactory {
public:
    ProjectAppearSwitchFactory();
    const char* convertName(const char* name) const override;
};

}  // namespace al
