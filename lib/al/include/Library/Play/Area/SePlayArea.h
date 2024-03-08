#include "al/include/Library/Area/AreaObj.h"

namespace al {
class SePlayArea : public AreaObj {
private:
    char* mSePlayName = nullptr;

public:
    SePlayArea(const char* name);

    void init(const AreaInitInfo& areaInitInfo) override;
};
}  // namespace al
