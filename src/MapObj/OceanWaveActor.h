#pragma once

#include <container/seadPtrArray.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class OceanWave;
class ParameterList;
class ParameterArray;
class FunctorBase;
}  // namespace al

class OceanWaveActor : public al::LiveActor {
public:
    OceanWaveActor(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void makeActorAlive() override;
    void makeActorDead() override;
    void control() override;

    void initOceanPreset();
    void initCloudPreset();
    void onSnapShotMode();
    void offSnapShotMode();
    bool tryLoadPreset() const;

    bool isSodaOcean() const;
    bool calcIsInArea(const sead::Vector3f& pos) const;
    void calcPos(sead::Vector3f* out, const sead::Vector3f& pos) const;
    void calcNormal(sead::Vector3f* out, const sead::Vector3f& pos) const;
    void calcVelocity(sead::Vector3f* out, const sead::Vector3f& pos) const;

private:
    al::OceanWave* mOceanWave = nullptr;
    s32 mConfigPreset = 0;
    sead::Matrix34f mBaseMatrix;
    const f32* mSeUserSyncParam = nullptr;
    s32 mConfigValue = 0;
    bool mIsCloudOcean = false;
    al::ParameterList* mMaterialPresetList = nullptr;
    al::ParameterArray* mMaterialPresetArray = nullptr;
    sead::PtrArrayImpl mMaterialPresets;
    al::ParameterList* mStageSettingList = nullptr;
    al::ParameterArray* mStageSettingArray = nullptr;
    sead::PtrArrayImpl mStageSettings;
};

static_assert(sizeof(OceanWaveActor) == 0x198);
