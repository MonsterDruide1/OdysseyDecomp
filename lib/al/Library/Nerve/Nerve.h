#pragma once

namespace al {
class NerveKeeper;

class Nerve {
public:
    virtual void execute(NerveKeeper* keeper) const = 0;

    virtual void executeOnEnd(NerveKeeper* keeper) const {}
};
}  // namespace al
