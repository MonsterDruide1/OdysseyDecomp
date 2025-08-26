#include "Library/Area/TrafficAreaDirector.h"

#include "Library/Area/AreaObjGroup.h"
#include "Library/Area/TrafficArea.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"

namespace al {
TrafficAreaDirector::TrafficAreaDirector(const AreaObjGroup* areaObjGroup,
                                         const ITrafficAreaUpdateJudge* trafficAreaUpdateJudge)
    : mTrafficAreaUpdateJudge(trafficAreaUpdateJudge) {
    mTrafficAreaCount = areaObjGroup->getSize();
    mTrafficAreas = new TrafficArea*[mTrafficAreaCount];

    for (s32 i = 0; i < mTrafficAreaCount; i++)
        mTrafficAreas[i] = (TrafficArea*)areaObjGroup->getAreaObj(i);
}

bool TrafficAreaDirector::tryPermitEnterCar(const sead::Vector3f& position) {
    TrafficArea* trafficArea = tryFindArea(position);

    if (!trafficArea)
        return true;

    return trafficArea->tryPermitEnterCar();
}

TrafficArea* TrafficAreaDirector::tryFindArea(const sead::Vector3f& position) const {
    for (s32 i = 0; i < mTrafficAreaCount; i++)
        if (mTrafficAreas[i]->isInVolume(position))
            return mTrafficAreas[i];

    return nullptr;
}

bool TrafficAreaDirector::tryPermitEnterNpcAndSyncDrawClipping(LiveActor* actor) {
    TrafficArea* trafficArea = tryFindArea(getTrans(actor));

    if (!trafficArea) {
        if (alActorFunction::isDrawClipping(actor))
            offDrawClipping(actor);

        return true;
    }

    if (!trafficArea->tryPermitEnterNpc())
        return false;

    if (!alActorFunction::isDrawClipping(actor))
        onDrawClipping(actor);

    return true;
}

void TrafficAreaDirector::execute() {
    if (mTrafficAreaUpdateJudge && !mTrafficAreaUpdateJudge->judge())
        return;

    for (s32 i = 0; i < mTrafficAreaCount; i++)
        mTrafficAreas[i]->reset();
}
}  // namespace al
