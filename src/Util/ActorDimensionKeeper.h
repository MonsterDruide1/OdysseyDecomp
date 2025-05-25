#pragma once

#include <basis/seadTypes.h>

namespace al {
class CollisionPartsFilterOnlySpecialPurpose;
class LiveActor;
}  // namespace al
class In2DAreaMoveControl;
class IUseDimension;

class ActorDimensionKeeper {
public:
    ActorDimensionKeeper(const al::LiveActor* actor);
    void validate();
    void invalidate();
    void forceChange2DKeep();
    void forceEndChange2DKeep();
    void update();

    bool getIs2D() const { return mIs2D; }

    bool getIsIn2DArea() const { return mIsIn2DArea; }

    bool getIsCurrently2D() const { return mIsCurrently2D; }

    bool getIsCanChange2D() const { return mIsCanChange2D; }

    bool getIsCanChange3D() const { return mIsCanChange3D; }

private:
    const al::LiveActor* mActor;
    bool mIsValid = true;
    bool mIs2D = false;
    bool mIsIn2DArea = false;
    bool mIsCurrently2D = false;
    bool mIsCanChange2D = false;
    bool mIsCanChange3D = false;
    In2DAreaMoveControl* mIn2DAreaMoveControl = nullptr;
};

static_assert(sizeof(ActorDimensionKeeper) == 0x18);
