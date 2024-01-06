#include "Library/LiveActor/ActorClippingFunction.h"

#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorFlag.h"

namespace al {
bool isClipped(const LiveActor* actor) {
    return actor->getFlags()->isClipped;
}
bool isInvalidClipping(const LiveActor* actor) {
    return actor->getFlags()->isClippingInvalid;
}
}  // namespace al

namespace alActorFunction {
bool isDrawClipping(const al::LiveActor* actor) {
    return actor->getFlags()->isDrawClipped;
}
}  // namespace alActorFunction
