#include "al/Library/Rail/RailRider.h"

#include "al/Library/Rail/Rail.h"

namespace al {
RailRider::RailRider(const Rail* rail) : mRail(rail), mCoord(rail->normalizeLength(0)) {
    rail->calcPosDir(&mPosition, &mDirection, mCoord);
}
}  // namespace al
