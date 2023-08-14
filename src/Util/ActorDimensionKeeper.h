#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}
class In2DAreaMoveControl;
class IUseDimension;

class ActorDimensionKeeper {
public:
    ActorDimensionKeeper(const al::LiveActor* actor);
    void validate();
    void invalidate();
    void forceChange2DKeep();
    void forceEndChange2DKeep();
    bool update();

    bool getIs2D() const { return is2D; }
    bool getIsIn2DArea() const { return isIn2DArea; }
    bool getIsCurrently2D() const { return isCurrently2D; }
    bool getIsCanChange2D() const { return isCanChange2D; }
    bool getIsCanChange3D() const { return isCanChange3D; }

private:
    const al::LiveActor* mLiveActor;
    bool isValid = true;
    bool is2D = false;
    bool isIn2DArea = false;
    bool isCurrently2D = false;
    bool isCanChange2D = false;
    bool isCanChange3D = false;
    In2DAreaMoveControl* mIn2DAreaMoveControl = nullptr;
};
static_assert(sizeof(ActorDimensionKeeper) == 0x18);

namespace rs {

ActorDimensionKeeper* createDimensionKeeper(const al::LiveActor* actor);
void updateDimensionKeeper(ActorDimensionKeeper* keeper);

bool is2D(const IUseDimension* dimension);
bool isIn2DArea(const IUseDimension* dimension);
bool isChange2D(const IUseDimension* dimension);
bool isChange3D(const IUseDimension* dimension);
bool is3D(const IUseDimension* dimension);
void snap2D(const al::LiveActor* actor, const IUseDimension* dimension, f32 unk_distance);

}  // namespace rs
