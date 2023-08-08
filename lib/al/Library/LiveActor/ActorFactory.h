#pragma once

#include "al/Library/Factory/Factory.h"

namespace al {
class LiveActor;

class ActorFactory : public al::Factory<al::LiveActor* (*)(char const*)> {
public:
    ActorFactory(const char*);
};
}  // namespace al
