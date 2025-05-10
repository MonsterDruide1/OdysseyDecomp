#pragma once

namespace al {

class IUseExecutor {
public:
    virtual void execute() = 0;

    virtual void draw() const {}
};

static_assert(sizeof(IUseExecutor) == 0x8);

}  // namespace al
