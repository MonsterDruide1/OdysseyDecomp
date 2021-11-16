#pragma once

class IUseDimension;

namespace al {
class LiveActor;
}

namespace rs {

bool is2D(const IUseDimension*);
bool isIn2DArea(const IUseDimension*);

bool isGuardNosePainCap(const al::LiveActor*);

}  // namespace rs
