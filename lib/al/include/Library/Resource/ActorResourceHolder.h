#pragma once

#include <prim/seadSafeString.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class Resource;
class ActorResource;

class ActorResourceHolder : public HioNode {
public:
    ActorResourceHolder(int);
    ~ActorResourceHolder();

    ActorResource* tryFindActorResource(const sead::SafeString&);
    ActorResource* findActorResourceImpl(const sead::SafeString&);
    ActorResource* createActorResource(const sead::SafeString&, Resource*, Resource*);
    void removeAll();
    void eraseResourceUser(Resource*);
    void freeErasedActorResource();

private:
    void* filler[9];
};
static_assert(sizeof(ActorResourceHolder) == 0x48);

}  // namespace al
