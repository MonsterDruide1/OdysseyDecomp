#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class EnemyStateBlowDown;
struct EnemyStateBlowDownParam;
}  // namespace al

class EnemyCap : public al::LiveActor {
public:
    static EnemyCap* createEnemyCap(const char* name);

    EnemyCap(const char* name);

    void initPartsFixFile(al::LiveActor* cap, const al::ActorInitInfo& info,
                          const char* archiveName, const char* suffix);

    void makeActorAlive() override;
    void updatePose();
    void calcAnim() override;

    void exeWait();
    bool syncHostVisible();
    void exeBlowDown();
    void startBlowDown(const al::HitSensor* source);
    void startBlowDown();
    bool isBlowDown() const;
    void setBlowDownParam(const al::EnemyStateBlowDownParam* param);

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
EnemyCap* tryCreateEnemyCap(al::LiveActor* actor, const al::ActorInitInfo& info);
EnemyCap* tryCreateEnemyCap(al::LiveActor* actor, const al::ActorInitInfo& info,
                            const char* archiveName);
EnemyCap* tryCreateEnemyCapSuffix(al::LiveActor* actor, const al::ActorInitInfo& info,
                                  const char* archiveName, const char* suffix);
bool tryStartEnemyCapBlowDown(EnemyCap* cap, const al::HitSensor* sensor);
bool tryStartEnemyCapBlowDown(EnemyCap* cap);
bool tryAppearEnemyCap(EnemyCap* cap);
bool isOnEnemyCap(EnemyCap* cap);
}  // namespace rs
