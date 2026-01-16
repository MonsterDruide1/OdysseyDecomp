#include "Util/ActorDimensionUtil.h"

#include "Util/ActorDimensionKeeper.h"
#include "Util/IUseDimension.h"

namespace rs {

ActorDimensionKeeper* createDimensionKeeper(const al::LiveActor* actor) {
    return new ActorDimensionKeeper(actor);
}

bool is2D(const IUseDimension* dimension) {
    return dimension->getActorDimensionKeeper()->is2D();
}

bool is3D(const IUseDimension* dimension) {
    ActorDimensionKeeper* keeper = dimension->getActorDimensionKeeper();
    return !keeper->is2D() && !keeper->isCurrently2D();
}

bool isChange2D(const IUseDimension* dimension) {
    return dimension->getActorDimensionKeeper()->isCanChange2D();
}

bool isChange3D(const IUseDimension* dimension) {
    return dimension->getActorDimensionKeeper()->isCanChange3D();
}

bool isIn2DArea(const IUseDimension* dimension) {
    return dimension->getActorDimensionKeeper()->isIn2DArea();
}

}  // namespace rs
