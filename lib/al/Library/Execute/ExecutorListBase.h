#pragma once

#include "Library/HostIO/HioNode.h"

namespace al {

class ExecutorListBase : public HioNode {
public:
    ExecutorListBase(const char* name);

    virtual ~ExecutorListBase() = default;
    virtual void executeList() const = 0;
    virtual bool isActive() const = 0;

    const char* getName() const { return mName; }

private:
    const char* mName;
};

static_assert(sizeof(ExecutorListBase) == 0x10);

}  // namespace al
