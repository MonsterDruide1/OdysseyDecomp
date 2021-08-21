#pragma once

#include "al/actor/Factory.h"

namespace al {

    class ProjectAppearSwitchFactory : public al::ActorFactory
    {
    public:
        ProjectAppearSwitchFactory();
        const char* convertName(const char*) const override;
    };

}