#pragma once

#include "Library/LiveActor/ActorFactory.h"

class ProjectAppearSwitchFactory : public al::ActorFactory {
public:
    ProjectAppearSwitchFactory();
    const char* convertName(const char* name) const override;
};
