#pragma once

#include <container/seadPtrArray.h>
#include <container/seadStrTreeMap.h>
#include <prim/seadSafeString.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class Resource;
class ActorResource;

class ActorResourceHolder : public IUseHioNode {
public:
    ActorResourceHolder(s32 capacity);

    virtual ~ActorResourceHolder() = default;

    ActorResource* tryFindActorResource(const sead::SafeString& name);
    ActorResource* findActorResourceImpl(const sead::SafeString& name);
    ActorResource* createActorResource(const sead::SafeString& name, Resource* modelResource,
                                       Resource* animResource);
    void removeAll();
    void eraseResourceUser(Resource* resource);
    void freeErasedActorResource();

private:
  sead::StrTreeMap<128, al::ActorResource*> mTreeMap;
  sead::PtrArray<ActorResource> mActorResource;
  sead::PtrArray<ActorResource> mErasedActorResource;
};

static_assert(sizeof(ActorResourceHolder) == 0x48);

}  // namespace al
