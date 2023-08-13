#pragma once

#include "Library/HostIO/HioNode.h"

namespace al {
class ActorResource;

class ModelKeeper : public al::HioNode {
public:
    ModelKeeper();

    virtual ~ModelKeeper();

    void initResource()
};
};  // namespace al
