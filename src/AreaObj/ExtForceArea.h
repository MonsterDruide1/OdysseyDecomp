#include "al/include/Library/Area/AreaObj.h"

class ExtForceArea : public al::AreaObj {
private:
    sead::Vector2f mUnknown;
    f32 mMagnitude = 1.0;

public:
    ExtForceArea(const char* name);

    void init(const al::AreaInitInfo& areaInitInfo) override;

    void calcExtForce(sead::Vector3f, sead::Vector3f, sead::Vector3f, sead::Vector3f);
};
