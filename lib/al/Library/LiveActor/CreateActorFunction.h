#pragma once

namespace al {
class LiveActor;

template <typename T>
LiveActor* createActorFunction(const char* actorName) {
    return new T(actorName);
}
}  // namespace al
