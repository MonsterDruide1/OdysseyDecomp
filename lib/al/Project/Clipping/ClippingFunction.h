#pragma once

namespace al {
class LiveActor;
}

namespace alClippingFunction {
void addToClippingTarget(al::LiveActor* actor);
void removeFromClippingTarget(al::LiveActor* actor);
}  // namespace alClippingFunction
