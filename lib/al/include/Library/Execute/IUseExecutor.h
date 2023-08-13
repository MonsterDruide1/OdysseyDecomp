#pragma once

namespace al {

class IUseExecutor {
public:
    virtual void execute() = 0;
    virtual void draw() const = 0;
};

}  // namespace al
