#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
class Resource;

class ActorParamHolder {
public:
    static const char* getYamlName();
    static ActorParamHolder* tryCreate(LiveActor* actor, const Resource* resource,
                                       const char* suffix);

    f32 findParamF32(const char*);

private:
    ActorParamHolder(LiveActor* actor, const Resource* resource, const char* suffix);
};
}  // namespace al
