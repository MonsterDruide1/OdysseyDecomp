#pragma once

#include "Library/HostIO/HioNode.h"

#include "Player/IPlayerModelChanger.h"

namespace al {
class LiveActor;
}
class PlayerModelHolder;
class PlayerPainPartsKeeper;
class PlayerCostumeInfo;
class IUseDimension;

class PlayerModelChangerHakoniwa : public IPlayerModelChanger, public al::HioNode {
public:
    PlayerModelChangerHakoniwa(const al::LiveActor*, PlayerModelHolder*, PlayerPainPartsKeeper*,
                               PlayerCostumeInfo*, const IUseDimension*);
    void initStartModel();
    const char* getModelName();
    void changeModel(al::LiveActor*);
    void update(bool, bool);
    void syncHost(bool);
    void syncShowHide(al::LiveActor*);
    void startDamageStopDemo();
    void syncHostDamageStopDemo(bool);
    s32 calcCostumeWarmLevel(s32);
    void syncModelBoneVisibility();

    void resetPosition() override;
    void hideModel() override;
    void hideSilhouette() override;
    void hideShadowMask() override;
    void showModel() override;
    void showSilhouette() override;
    void showShadowMask() override;
    bool isFireFlower() override;
    bool isMini() override;
    bool isChange() override;
    bool is2DModel() override;
    bool isHiddenModel() override;
    bool isHiddenShadowMask() override;

private:
    const al::LiveActor* mLiveActor;
    bool mIsChange;
    bool mModeIs2D;
    al::LiveActor* mLiveActor2;
    PlayerModelHolder* mPlayerModelHolder;
    PlayerPainPartsKeeper* mPlayerPainPartsKeeper;
    PlayerCostumeInfo* mPlayerCostumeInfo;
    const IUseDimension* mIUseDimension;
    bool mVisibilityNeedsSync;
    bool mIsModelVisible;
    bool mIsSilhouetteVisible;
    bool mIsShadowMaskVisible;
    bool mIsBlinkingFromDamage;
    s32 mDamageTimer;
    bool mMusicStarted;
    bool mIsNeedHairControl;
    bool mIsNeedSyncBodyHair;
    bool mIsSyncFaceBeard;
    bool mIsSyncStrap;
    bool mIsPreventHeadPain;
};
