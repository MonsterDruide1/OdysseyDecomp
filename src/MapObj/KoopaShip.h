#pragma once

#include <math/seadMatrix.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
}
class KoopaShipWeddingPartsInfo;
class Peach;
class PropellerRotateInfo;

class KoopaShip : public al::LiveActor {
public:
    KoopaShip(const char*);
    void init(const al::ActorInitInfo& info) override;
    void onSwitchGateKeeperOpeningDemoStart();
    void onBlurMovie();
    void initAfterPlacement() override;
    void offDepthShadow();
    void onDepthShadow();
    void movement() override;
    void control() override;
    void appear() override;
    void kill() override;
    void calcAnim() override;
    void startClipped() override;
    void endClipped() override;
    void exeWait();
    void endWait();
    void exeDemo();
    void exeDemoAppear();
    void exeDemoGateKeeper();
    void exeDemoHomeFlyAway();
    void exeDemoHomeFlyAwayAfter();
    void startDemoWait();
    void endDemoWait(Peach*);
    void skipGateKeeperOpeningDemo();
    void startDemoHomeFlyAway();
    bool isEnableStartWipeDemoHomeFlyAway() const;
    s32 getWipeDemoFlyAwayStep();
    bool isEnableEndDemoHomeFlyAway() const;
    static KoopaShip* createForKoopaLv1(al::LiveActor*, Peach*, const al::ActorInitInfo&);
    void switchHalfModel();

private:
    const char* _108;
    void* filler_110[9];
    KoopaShipWeddingPartsInfo** _158;
    void* filler_160[2];
    sead::Matrix34f _170;
    sead::Matrix34f _1a0;
    void* filler_1d0[4];
};

namespace KoopaShipFunction {
al::LiveActor* tryCreateLinkKoopaShip(const al::ActorInitInfo&);
s32 getPropellerRotateInfoNum();
PropellerRotateInfo* getPropellerRotateInfo(s32);
const char* getWeddingPartsInitFileName();
}  // namespace KoopaShipFunction

static_assert(sizeof(KoopaShip) == 0x1f0);
