#pragma once

#include <prim/seadSafeString.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class Resource;
class InitResourceDataAnim;
class InitResourceDataAction;

class ActorResource : public IUseHioNode {
public:
    ActorResource(const sead::SafeString& name, Resource* modelResource, Resource* animResource);

    virtual ~ActorResource() = default;

    void initResourceData(const char* suffix, bool mergeAnim);

    const sead::SafeString& getName() const { return mName; }

    Resource* getModelRes() const { return mModelRes; }

    Resource* getAnimRes() const { return mAnimRes; }

    bool hasAnimData() const { return mHasAnimData; }

private:
    sead::FixedSafeString<128> mName = {""};
    Resource* mModelRes;
    Resource* mAnimRes;
    bool mHasAnimData = false;
    InitResourceDataAnim* mAnimResData = nullptr;
    InitResourceDataAction* mActionResData = nullptr;
};

}  // namespace al
