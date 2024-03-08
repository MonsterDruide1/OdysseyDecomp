#include "al/include/Library/Area/AreaObj.h"

class StainArea : public al::AreaObj {
private:
    s32 mStainType = 0;

public:
    StainArea(const char* name);

    void init(const al::AreaInitInfo& areaInitInfo) override;
};
