#pragma once

#include "Library/Area/AreaObjGroup.h"

namespace al {
class AreaObjDirector;
class PlayerHolder;

class ClippingFarAreaObserver {
public:
    ClippingFarAreaObserver(const AreaObjDirector* areaObjDirector,
                            const PlayerHolder* playerHolder);
    void endInit();
    void setDefaultFarClipDistance(f32 distance);
    void setDefaultFarClipDistanceSub(f32 distance);
    void update();

private:
    const AreaObjDirector* mAreaObjDirector;
    const PlayerHolder* mPlayerHolder;
    const AreaObjGroup* mAreaObjGroup = nullptr;
    const AreaObj* mAreaObj = nullptr;
    f32 mDefaultFarClipDistanceHorizontal = 7000.0f;
    f32 mDefaultFarClipDistanceVertical = 7000.0f;
    f32 mDefaultFarClipDistanceHorizontalSub = 4000.0f;
    f32 mDefaultFarClipDistanceVerticalSub = 4000.0f;
>>>>>>> 01c3dd20 (Implemented ClippingFarAreaObserver)
=======
    f32 mFarClipDistance = 7000.0f;
    f32 mDefaultFarClipDistance = 7000.0f;
    f32 mFarClipDistanceSub = 4000.0f;
    f32 mDefaultFarClipDistanceSub = 4000.0f;
>>>>>>> 3aa4b1c0 (Addressed review)
};
}  // namespace al
