#include "Library/Rail/RailRider.h"

#include "Library/Rail/Rail.h"

namespace al {
RailRider::RailRider(const Rail* rail) : mRail(rail), mCoord(rail->normalizeLength(0)) {
    rail->calcPosDir(&mPosition, &mDirection, mCoord);
}
}  // namespace al
