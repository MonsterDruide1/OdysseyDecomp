#include "al/include/Library/Area/AreaObj.h"

class NpcForceMaterialCodeArea : public al::AreaObj {
private:
    char* mMaterialCodeName = nullptr;

public:
    NpcForceMaterialCodeArea(const char* name);

    void init(const al::AreaInitInfo& areaInitInfo) override;
};
