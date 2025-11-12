#pragma once

#include <basis/seadTypes.h>

#include "Library/Message/IUseMessageSystem.h"
#include "Library/Nerve/NerveStateBase.h"

namespace al {
class Scene;
class SimpleLayoutAppearWaitEnd;
class LayoutActor;
class WindowConfirm;
class KeyRepeatCtrl;
class MessageSystem;
class WaveVibrationHolder;
class Scene;
class LayoutInitInfo;
}  // namespace al
class FooterParts;

class SceneStateSelectMode : public al::HostStateBase<al::Scene>, public al::IUseMessageSystem {
public:
    SceneStateSelectMode(const char*, al::Scene*, const al::LayoutInitInfo&, FooterParts*,
                         al::WindowConfirm*, al::WaveVibrationHolder*);
    ~SceneStateSelectMode() = default;

    void appear() override;
    void kill() override;

    void updateCursorParts(bool);
    void updateCursorPartsTrans();

    void exeAppear();
    void exeWait();
    void exeDecide();
    void exeDecideWait();
    void exeDecideConfirm();
    void exeEnd();

private:
    al::SimpleLayoutAppearWaitEnd* mModeSelect = nullptr;
    al::LayoutActor* mNormalModeButton = nullptr;
    al::LayoutActor* mKidsModeButton = nullptr;
    al::LayoutActor* mModeSelectCursor = nullptr;
    FooterParts* mFooterParts = nullptr;
    al::WindowConfirm* mWindowConfirm = nullptr;
    u8 mClosingTimer = 0;
    bool mIsNormal = false;
    al::KeyRepeatCtrl* mKeyRepeatCtrl;
    al::MessageSystem* mMessageSystem;
    al::WaveVibrationHolder* mWaveVibrationHolder;
};
