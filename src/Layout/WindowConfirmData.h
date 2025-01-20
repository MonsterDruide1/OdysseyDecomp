#pragma once

#include "Library/Nerve/NerveExecutor.h"

namespace nn::ui2d {
class TextureInfo;
}

namespace al {
class LayoutActor;
class LayoutInitInfo;
class SimpleLayoutAppearWaitEnd;
}  // namespace al

class WindowConfirmData : public al::NerveExecutor {
public:
    enum PaneType : s32 { PaneType_Confirm = 0, PaneType_Cancel = 1 };

    WindowConfirmData(const al::LayoutInitInfo& info, const char* layoutName, const char* name,
                      bool createDataParts);

    void setConfirmMessage(const char16* message, const char16* confirmMessage,
                           const char16* cancelMessage);
    void setConfirmData(al::LayoutActor* actor, nn::ui2d::TextureInfo* textureInfo);
    void updateConfirmDataDate();
    void appear();
    void appearWithChoicingCancel();
    void end();
    void kill();
    bool isEndSelect();
    bool isDecided();
    bool isCanceled();
    bool isDisable();
    void exeAppear();
    void changeSelectingIdx(s32 index);
    void exeWait();
    void updateCursorPos();
    void exeSelect();
    void exeVanish();
    void exeDisable();

private:
    al::SimpleLayoutAppearWaitEnd* mWindowConfirmLayout = nullptr;
    al::LayoutActor* mParCursor = nullptr;
    al::LayoutActor* mParOptions[2] = {nullptr, nullptr};
    al::LayoutActor* mParData = nullptr;
    PaneType mSelectionIndex = PaneType::PaneType_Confirm;
    s32 mSelectionCooldown = 0;
};
