#include "Project/Clipping/ClippingFunction.h"

#include "Library/LiveActor/LiveActor.h"
#include "Library/Scene/Scene.h"
#include "Project/Clipping/ClippingDirector.h"

namespace alClippingFunction {
void addToClippingTarget(al::LiveActor* actor) {
    actor->getSceneInfo()->clippingDirector->getClippingActorHolder()->addToClippingTarget(actor);
}

void removeFromClippingTarget(al::LiveActor* actor) {
    actor->getSceneInfo()->clippingDirector->getClippingActorHolder()->removeFromClippingTarget(
        actor);
}

}  // namespace alClippingFunction
