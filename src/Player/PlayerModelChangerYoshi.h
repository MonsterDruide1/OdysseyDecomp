#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

#include "Player/IPlayerModelChanger.h"

namespace al {
class LiveActor;
}  // namespace al

class PlayerModelHolder;

class PlayerModelChangerYoshi : public al::HioNode, public IPlayerModelChanger {
public:
    PlayerModelChangerYoshi(const al::LiveActor* host, PlayerModelHolder* modelHolder);

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

    void syncHost();
    void syncModelFlag(al::LiveActor* actor);
    void appearModel();
    void killModel();
    void changeModel(al::LiveActor* actor);

private:
    const al::LiveActor* mHost = nullptr;
    al::LiveActor* mModelActor = nullptr;
    PlayerModelHolder* mModelHolder = nullptr;
    u16 mModelFlags = 0;
    bool mIsSilhouetteVisible = false;
    u8 _23[0x35] = {};
};

static_assert(sizeof(PlayerModelChangerYoshi) == 0x58);
