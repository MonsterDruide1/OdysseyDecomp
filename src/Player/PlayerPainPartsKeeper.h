#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
class ActorInitInfo;
class PartsModel;
}  // namespace al
class PlayerCostumeInfo;
class PlayerModelHolder;

class PlayerPainPartsKeeper {
public:
    PlayerPainPartsKeeper(const al::LiveActor*, const PlayerCostumeInfo*);
    void update();
    void updateNeedle();
    void resetPosition();
    bool isEnableNosePain() const;
    bool isInvalidNoseDynamics() const;
    void createNoseNeedle(const PlayerModelHolder*, const al::ActorInitInfo&);
    void appearNeedle();

private:
    const al::LiveActor* mLiveActor;
    const PlayerCostumeInfo* mPlayerCostumeInfo;
    f32 mModelAlphaMask = 1;
    bool mEnableTimer = true;
    al::LiveActor* mPlayerFaceActor = nullptr;
    al::PartsModel* mNeedlesActor = nullptr;
    s32 mTimer = 0;
};
