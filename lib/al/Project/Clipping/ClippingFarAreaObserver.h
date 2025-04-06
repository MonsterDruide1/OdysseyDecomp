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
};
}  // namespace al
