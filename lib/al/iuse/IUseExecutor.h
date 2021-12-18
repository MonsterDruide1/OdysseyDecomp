#pragma once

namespace al {

class IUseExecutor {
public:
    virtual void execute();
    virtual void draw() const;
};

}  // namespace al
