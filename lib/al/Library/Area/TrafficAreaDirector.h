#pragma once

#include <math/seadVector.h>

#include "Library/Execute/IUseExecutor.h"
#include "Library/HostIO/HioNode.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
class AreaObjGroup;
class ITrafficAreaUpdateJudge;
class LiveActor;
class TrafficArea;

class ITrafficAreaUpdateJudge {
public:
    virtual bool judge() const = 0;
};

class TrafficAreaDirector : public HioNode, public IUseExecutor, public ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_alTrafficAreaDirector;

    TrafficAreaDirector(const AreaObjGroup* areaObjGroup,
                        const ITrafficAreaUpdateJudge* trafficAreaUpdateJudge);

    void execute() override;

    const char* getSceneObjName() const override { return "交通整理エリア管理"; }

    bool tryPermitEnterCar(const sead::Vector3f& position);
    TrafficArea* tryFindArea(const sead::Vector3f& position) const;
    bool tryPermitEnterNpcAndSyncDrawClipping(LiveActor* actor);

private:
    s32 mTrafficAreaCount = 0;
    TrafficArea** mTrafficAreas = nullptr;
    const ITrafficAreaUpdateJudge* mTrafficAreaUpdateJudge;
};

static_assert(sizeof(TrafficAreaDirector) == 0x28);
}  // namespace al
