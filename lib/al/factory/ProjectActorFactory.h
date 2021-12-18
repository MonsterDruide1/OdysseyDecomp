#pragma once

#include "al/factory/Factory.h"

namespace al {
class ProjectActorFactory : public al::ActorFactory {
public:
    ProjectActorFactory();
    const char* convertName(const char*) const override;
};
}  // namespace al
