#pragma once

#include "Player/IPlayerModelChanger.h"

class PlayerModelHolder;

namespace al {
class LiveActor;
}

class PlayerModelChangerYoshi : public IPlayerModelChanger {
public:
    PlayerModelChangerYoshi(const al::LiveActor*, PlayerModelHolder*);

    bool isFireFlower() const override;
    bool isMini() const override;
    bool isChange() const override;
    bool is2DModel() const override;
    bool isHiddenModel() const override;
    bool isHiddenShadowMask() const override;

    void resetPosition() override;
    void hideModel() override;
    void hideSilhouette() override;
    void hideShadowMask() override;
    void showModel() override;
    void showSilhouette() override;
    void showShadowMask() override;

    void appearModel();
    void killModel();
    void syncHost();

    void changeModel(al::LiveActor*);
    void syncModelFlag(al::LiveActor*);
};
