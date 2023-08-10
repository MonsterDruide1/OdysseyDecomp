#pragma once

#include "al/Library/Factory/Factory.h"

namespace al {
class LiveActor;

class ActorFactory : public Factory<CreatorFunction<LiveActor>> {
public:
    ActorFactory(const char* factoryName);
};
}  // namespace al
