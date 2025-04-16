#pragma once

namespace al {

class AreaObjDirector;
class PlayerHolder;

class ClippingFarAreaObserver {
public:
    ClippingFarAreaObserver(const AreaObjDirector*, const PlayerHolder*);
    void setDefaultFarClipDistance(f32);
    void setDefaultFarClipDistanceSub(f32);
    void endInit();
    void update();
};
}  // namespace al
