#pragma once

#include "Library/LiveActor/ActorFactory.h"

namespace al {

class ProjectActorFactory : public al::ActorFactory {
public:
    ProjectActorFactory();
    const char* convertName(const char* name) const override;
};

}  // namespace al
