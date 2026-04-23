#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "MapObj/RouteGuidePoint.h"

namespace al {
struct ActorInitInfo;
}

class HintRouteGuidePoint : public al::LiveActor, public RouteGuidePoint {
public:
    HintRouteGuidePoint(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;

    const sead::Vector3f& getGuideTrans() const override;
    bool isValidateGuide() const override;
    bool isHorizontalGuide() const override;

    void exeStart();
    bool tryGuideEnd();
    bool tryGuideHome();
    bool tryGuideNearHint();
    bool tryGuideDedication();
    void exeEnd();
    void exeHome();
    void exeNearHint();
    bool updateNearHint();
    void exeDedication();
    bool isEndDedication() const;
    void exeHintNpc();

private:
    sead::Vector3f mGuideTrans = sead::Vector3f::zero;
};

static_assert(sizeof(HintRouteGuidePoint) == 0x120);
