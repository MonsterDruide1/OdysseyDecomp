#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
class Resource;

class ModelOcclusionQuery {
public:
    static ModelOcclusionQuery* tryCreate(LiveActor*, Resource*, const char*);

    ModelOcclusionQuery();
    ~ModelOcclusionQuery();

    void update();
    u64 getZPassCount() const;
    bool isInside(const sead::Vector3f&) const;
};

}  // namespace al
