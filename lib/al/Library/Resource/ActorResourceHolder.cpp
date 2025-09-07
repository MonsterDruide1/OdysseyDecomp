#include "Library/Resource/ActorResourceHolder.h"

#include "Library/Resource/ActorResource.h"

namespace al {

ActorResourceHolder::ActorResourceHolder(s32 capacity) {
    mTreeMap.allocBuffer(capacity, nullptr);
    mActorResource.allocBuffer(capacity, nullptr);
    mErasedActorResource.allocBuffer(capacity, nullptr);
}

ActorResource* ActorResourceHolder::tryFindActorResource(const sead::SafeString& name) {
    return findActorResourceImpl(name);
}

ActorResource* ActorResourceHolder::findActorResourceImpl(const sead::SafeString& name) {
    auto* node = mTreeMap.find(name);
    if (!node)
        return nullptr;
    return node->value();
}

ActorResource* ActorResourceHolder::createActorResource(const sead::SafeString& name,
                                                        Resource* modelResource,
                                                        Resource* animResource) {
    ActorResource* resource = new ActorResource(name, modelResource, animResource);
    mTreeMap.insert(resource->getName(), resource);
    mActorResource.pushBack(resource);
    return resource;
}

void ActorResourceHolder::removeAll() {
    mTreeMap.clear();
    mActorResource.clear();
}

void ActorResourceHolder::eraseResourceUser(Resource* resource) {
    for (ActorResource& res : mActorResource) {
        if (res.getModelRes() != resource)
            continue;

        mActorResource.erase(mActorResource.indexOf(&res));
        mErasedActorResource.pushBack(&res);
        if (mTreeMap.find(res.getName()))
            mTreeMap.erase(res.getName());
    }
}

void ActorResourceHolder::freeErasedActorResource() {
    for (ActorResource& it : mErasedActorResource) {
        ActorResource* res = &it;
        // compiler must forget about being a reference above,
        // otherwise it assumes that res is not null
        __asm("" : "=r"(res) : "0"(res));

        if (!res )
            continue;
        delete res;
    }
    mErasedActorResource.clear();
}

}  // namespace al
