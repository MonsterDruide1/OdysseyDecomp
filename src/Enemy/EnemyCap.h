#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class EnemyStateBlowDown;
class HitSensor;
struct EnemyStateBlowDownParam;
}  // namespace al

class EnemyCap : public al::LiveActor {
public:
    static EnemyCap* createEnemyCap(const char*);

    EnemyCap(const char*);

    void initPartsFixFile(al::LiveActor*, const al::ActorInitInfo&, const char*, const char*);

    void makeActorAlive() override;
    void updatePose();
    void calcAnim() override;

    void exeWait();
    bool syncHostVisible();
    void exeBlowDown();
    void startBlowDown(const al::HitSensor* source);
    void startBlowDown();
    bool isBlowDown() const;
    void setBlowDownParam(const al::EnemyStateBlowDownParam*);

    al::LiveActor* getCap() { return mCap; };

    al::EnemyStateBlowDown* getStateBlowDown() { return mStateBlowDown; };

private:
    al::LiveActor* mCap = nullptr;
    const sead::Matrix34f* mCapBaseMtx = nullptr;
    bool mIsNotAtOrigin = true;
    sead::Vector3f mLocalTrans = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mLocalRotate = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mLocalScale = {1.0f, 1.0f, 1.0f};
    bool mIsCapVisible = false;
    bool mIsUseFollowMtxScale = false;
    bool field_142 = false;
    bool mIsUseLocalScale = true;
    al::EnemyStateBlowDown* mStateBlowDown = nullptr;
};

namespace rs {
EnemyCap* tryCreateEnemyCap(al::LiveActor*, const al::ActorInitInfo&);
EnemyCap* tryCreateEnemyCap(al::LiveActor*, const al::ActorInitInfo&, const char*);
EnemyCap* tryCreateEnemyCapSuffix(al::LiveActor*, const al::ActorInitInfo&, const char*,
                                  const char*);
bool tryStartEnemyCapBlowDown(EnemyCap*, const al::HitSensor*);
bool tryStartEnemyCapBlowDown(EnemyCap*);
bool tryAppearEnemyCap(EnemyCap*);
bool isOnEnemyCap(EnemyCap*);
}  // namespace rs
