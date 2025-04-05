#pragma once

#include <basis/seadTypes.h>

namespace al {

class PlayerHolder;
class AreaObjDirector;

class ClippingFarAreaObserver {
public:
    ClippingFarAreaObserver(const AreaObjDirector*, const PlayerHolder*);
    void endInit();
    void setDefaultFarClipDistance(f32);
    void setDefaultFarClipDistanceSub(f32);
    void update();
};
}  // namespace al
