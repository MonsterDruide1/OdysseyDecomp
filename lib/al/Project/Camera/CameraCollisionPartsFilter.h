#pragma once

#include "Library/Collision/KCollisionServer.h"

namespace al {

class CameraCollisionPartsFilter : public CollisionPartsFilterBase {
public:
    CameraCollisionPartsFilter();

    bool isInvalidParts(const CollisionParts&) const override;
};

static_assert(sizeof(CameraCollisionPartsFilter) == 0x8);

}  // namespace al
