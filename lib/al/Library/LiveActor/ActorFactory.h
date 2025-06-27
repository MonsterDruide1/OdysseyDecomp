#pragma once

#include "Library/Factory/Factory.h"

namespace al {
class LiveActor;

using ActorCreatorFunction = LiveActor* (*)(const char* actorName);

class ActorFactory : public Factory<ActorCreatorFunction> {
public:
    ActorFactory(const char* pName);
};
}  // namespace al
