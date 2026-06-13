#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Enemy/Bros.h"

class HammerBrosHammer : public BrosWeaponBase {
public:
    HammerBrosHammer(const char* name, const al::LiveActor* bros, const char* archiveName,
                     bool isSearch);

    void init(const al::ActorInitInfo& initInfo) override;
    void shoot(const sead::Vector3f& trans, const sead::Quatf& quat, const sead::Vector3f& force,
               bool isHack, s32 unknown, bool isFast) override;
    void killCollide(al::HitSensor* sensor, const sead::Vector3f& trans, bool isHack) override;
    void killEnemy() override;
    void attach(const sead::Matrix34f* poseMtx, const sead::Vector3f& trans,
                const sead::Vector3f& rotation, const char* actionName) override;
    bool isBreak() const override;
    u32 getBreakStep() const override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeAttach();
    void exeMove();
    void exeBreak();

private:
    bool mIsHack = false;
    const char* mArchiveName;
    sead::Matrix34f mSurfaceMtx = sead::Matrix34f::ident;
    bool mIsSearch;
    s32 mSearchCooldown = 0;
};

static_assert(sizeof(HammerBrosHammer) == 0x178);
