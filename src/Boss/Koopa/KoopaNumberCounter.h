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
    void addBeamCountAndCheckEnd();
    bool isEnableStartDamageBall() const;
    void tryStartDamageBall();
    bool isBombDamageBall(s32) const;
    void addDamageBallCountAndCheckEnd();
    void addDamageBallContinueSkipCountAndCheckSkip();
    bool isDamageBallContinueSkipped() const;
    void tryStartAttackTail(KoopaCap*);
    bool isExecuteAttackTailTwice() const;
    void addAttackTailCountAndCheckEnd();
    bool isGuardPunchHitAction() const;
    void* getPunchHitSeParamPtr() const;
    void addPunchHitCountAndPowerUpCap(KoopaCap*);
    void recoverPunchHitCount(KoopaCap*);
    void startPunchHitAction(al::LiveActor*) const;
    void makeDamageAnimSeName(sead::BufferedSafeStringBase<char>*) const;
    void receiveDamageAndCheckDead(KoopaCap*);
};
