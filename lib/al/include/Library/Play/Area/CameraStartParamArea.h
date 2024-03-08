#include "al/include/Library/Area/AreaObj.h"

namespace al {
class CameraStartParamArea : public al::AreaObj {
private:
    bool mIsAlive = true;
    f32* mAngleH = nullptr;
    f32* mAngleV = nullptr;

public:
    CameraStartParamArea(const char* name);

    void init(const al::AreaInitInfo& areaInitInfo) override;

    void appear();
    void kill();
};
}  // namespace al
