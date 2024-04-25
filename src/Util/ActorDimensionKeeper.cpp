#include "Util/ActorDimensionKeeper.h"

#include "Util/IUseDimension.h"

void ActorDimensionKeeper::validate() {
    mIsValid = true;
}

void ActorDimensionKeeper::invalidate() {
    mIsValid = false;

    if (mIs2D) {
        mIs2D = false;
        mIsIn2DArea = false;
        mIsCurrently2D = false;
        mIsCanChange3D = true;
    }
}

namespace rs {

ActorDimensionKeeper* createDimensionKeeper(const al::LiveActor* actor) {
    return new ActorDimensionKeeper(actor);
}

bool is2D(const IUseDimension* dimension) {
    return dimension->getActorDimensionKeeper()->getIs2D();
}

bool is3D(const IUseDimension* dimension) {
    ActorDimensionKeeper* keeper = dimension->getActorDimensionKeeper();
    return !keeper->getIs2D() && !keeper->getIsCurrently2D();
}

bool isChange2D(const IUseDimension* dimension) {
    return dimension->getActorDimensionKeeper()->getIsCanChange2D();
}

bool isChange3D(const IUseDimension* dimension) {
    return dimension->getActorDimensionKeeper()->getIsCanChange3D();
}

bool isIn2DArea(const IUseDimension* dimension) {
    return dimension->getActorDimensionKeeper()->getIsIn2DArea();
}

}  // namespace rs
