#pragma once

#include "Library/HostIO/HioNode.h"

#include "Player/IPlayerModelChanger.h"

namespace al {
class LiveActor;
}  // namespace al

class PlayerModelHolder;

class PlayerModelChangerYoshi : public al::HioNode, public IPlayerModelChanger {
public:
    PlayerModelChangerYoshi(const al::LiveActor*, PlayerModelHolder*);

    void resetPosition() override;
    void hideModel() override;
    void hideSilhouette() override;
    void showModel() override;
    void showSilhouette() override;
    bool isHiddenModel() const override;

    bool isFireFlower() const override { return false; }

    bool isMini() const override { return false; }

    bool isChange() const override { return false; }

    bool is2DModel() const override { return false; }

    bool isHiddenShadowMask() const override { return false; }

    void hideShadowMask() override {}

    void showShadowMask() override {}

    void syncHost();
    void syncModelFlag(al::LiveActor*);
    void appearModel();
    void killModel();
    void changeModel(al::LiveActor*);

private:
    al::LiveActor* mActor;
    al::LiveActor* mModelActor;
    PlayerModelHolder* mModelHolder;
    bool _20;
    bool _21;
    bool _22;
};

static_assert(sizeof(PlayerModelChangerYoshi) == 0x28);
