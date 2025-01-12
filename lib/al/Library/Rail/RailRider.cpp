#include "Library/Rail/RailRider.h"

#include "Library/Rail/Rail.h"

namespace al {

// NON_MATCHING

RailRider::RailRider(const Rail* rail) : mRail(rail), mCoord(rail->normalizeLength(0)) {
    rail->calcPosDir(&mPosition, &mDirection, mCoord);
}
}  // namespace al
