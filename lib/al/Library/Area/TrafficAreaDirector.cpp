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
    for (s32 i = 0; i < mTrafficAreaCount; i++) {
        if (!mTrafficAreas[i]->isInVolume(position))
            continue;

        if (!mTrafficAreas[i])
            return true;

        return mTrafficAreas[i]->tryPermitEnterCar();
    }

    return true;
}

TrafficArea* TrafficAreaDirector::tryFindArea(const sead::Vector3f& position) const {
    for (s32 i = 0; i < mTrafficAreaCount; i++) {
        if (!mTrafficAreas[i]->isInVolume(position))
            continue;

        return mTrafficAreas[i];
    }

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

    if (alActorFunction::isDrawClipping(actor))
        return true;

    onDrawClipping(actor);

    return true;
}

void TrafficAreaDirector::execute() {
    if (mTrafficAreaUpdateJudge && !mTrafficAreaUpdateJudge->judge())
        return;

    for (s32 i = 0; i < mTrafficAreaCount; i++) {
        TrafficArea* trafficArea = mTrafficAreas[i];

        trafficArea->reset();
    }
}

const char* TrafficAreaDirector::getSceneObjName() const {
    return "交通整理エリア管理";
}
}  // namespace al
