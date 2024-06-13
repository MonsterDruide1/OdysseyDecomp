#pragma once

namespace al {
class NerveKeeper;

class IUseNerve {
public:
    inline IUseNerve() {}

    virtual NerveKeeper* getNerveKeeper() const = 0;
};
}  // namespace al
