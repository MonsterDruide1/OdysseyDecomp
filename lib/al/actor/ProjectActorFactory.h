#pragma once

#include "al/actor/Factory.h"

namespace al
{
    class ProjectActorFactory : public al::ActorFactory
    {
    public:
        ProjectActorFactory();
        const char* convertName(const char*) const override;
    };
};