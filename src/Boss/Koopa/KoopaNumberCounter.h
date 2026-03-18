#pragma once

#include <prim/seadSafeString.h>

namespace al {
class LiveActor;
struct ActorInitInfo;
}  // namespace al

class KoopaCap;

class KoopaNumberCounter {
public:
    KoopaNumberCounter(const al::ActorInitInfo&);
    bool isValidFireBeam() const;
    s32 getDamage() const;
    bool isShowCapWide() const;
    void startBeam();
    bool isFirstBeam() const;
    bool isLastBeam() const;
    bool addBeamCountAndCheckEnd();
    bool isEnableStartDamageBall() const;
    bool tryStartDamageBall();
    bool isBombDamageBall(s32) const;
    bool addDamageBallCountAndCheckEnd();
    bool addDamageBallContinueSkipCountAndCheckSkip();
    bool isDamageBallContinueSkipped() const;
    bool tryStartAttackTail(KoopaCap*);
    bool isExecuteAttackTailTwice() const;
    bool addAttackTailCountAndCheckEnd();
    bool isGuardPunchHitAction() const;
    f32* getPunchHitSeParamPtr() const;
    void addPunchHitCountAndPowerUpCap(KoopaCap*);
    void recoverPunchHitCount(KoopaCap*);
    void startPunchHitAction(al::LiveActor*) const;
    void makeDamageAnimSeName(sead::BufferedSafeString*) const;
    bool receiveDamageAndCheckDead(KoopaCap*);
};
