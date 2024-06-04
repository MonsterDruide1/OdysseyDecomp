#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class EnemyStateBlowDown;
struct EnemyStateBlowDownParam;
}  // namespace al

class EnemyCap : public al::LiveActor {
private:
    al::LiveActor* mCap = nullptr;
    sead::Matrix34f* mCapBaseMtx = nullptr;
    bool mNotAtOrigin = true;
    sead::Vector3f mLocalTrans = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mLocalRotate = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mLocalScale = {1.0f, 1.0f, 1.0f};
    bool mCapVisible = false;
    bool mUseFollowMtxScale = false;
    bool field_142 = false;
    bool mUseLocalScale = true;
    al::EnemyStateBlowDown* mStateBlowDown = nullptr;

public:
    static EnemyCap* createEnemyCap(const char*);

    EnemyCap(const char*);
    void initPartsFixFile(al::LiveActor*, const al::ActorInitInfo&, const char*, const char*);
    void makeActorAlive();
    void updatePose();
    void calcAnim();
    void exeWait();
    bool syncHostVisible();
    void exeBlowDown();
    void startBlowDown(const al::HitSensor* source);
    void startBlowDown();
    bool isBlowDown() const;
    void setBlowDownParam(const al::EnemyStateBlowDownParam*);

    al::LiveActor* getCap() { return mCap; };
    al::EnemyStateBlowDown* getStateBlowDown() { return mStateBlowDown; };
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