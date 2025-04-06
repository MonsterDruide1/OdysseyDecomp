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

    f32 getFarClipDistance() { return mFarClipDistance; }

private:
    const AreaObjDirector* mAreaObjDirector;
    const PlayerHolder* mPlayerHolder;
    const AreaObjGroup* mAreaObjGroup = nullptr;
<<<<<<< HEAD
    AreaObj* mAreaObj = nullptr;
=======
    const AreaObj* mAreaObj = nullptr;
>>>>>>> 9128333a (Stashing as im dealing with multiple pr's rn)
    f32 mFarClipDistance = 7000.0f;
    f32 mDefaultFarClipDistance = 7000.0f;
    f32 mFarClipDistanceSub = 4000.0f;
    f32 mDefaultFarClipDistanceSub = 4000.0f;
};
}  // namespace al
