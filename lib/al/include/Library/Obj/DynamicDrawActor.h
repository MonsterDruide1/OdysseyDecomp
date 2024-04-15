#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {

class IUseFinalize {
public:
    virtual void finalize() = 0;
};

class DynamicDrawActor : public LiveActor, public IUseFinalize {
public:
    // incomplete
    void finalize() override;

private:
    // missing
};

}  // namespace al
