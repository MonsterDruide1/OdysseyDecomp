#pragma once

#include <basis/seadTypes.h>

namespace al {
class IUseSceneObjHolder;
}  // namespace al

class InputSeparator {
public:
    InputSeparator(const al::IUseSceneObjHolder* sceneObjHolder, bool isVertical);

    void reset();
    void update();
    void updateForSnapShotMode();
    bool isTriggerUiLeft();
    bool checkDominant(bool isVertical);
    bool isTriggerUiRight();
    bool isTriggerUiUp();
    bool isTriggerUiDown();
    bool isHoldUiLeft();
    bool isHoldUiRight();
    bool isHoldUiUp();
    bool isHoldUiDown();
    bool isRepeatUiLeft();
    bool isRepeatUiRight();
    bool isRepeatUiUp();
    bool isRepeatUiDown();
    bool isTriggerSnapShotMode();
    bool isTriggerIncrementPostProcessingFilterPreset();
    bool isTriggerDecrementPostProcessingFilterPreset();

private:
    const al::IUseSceneObjHolder* mSceneObjHolder;
    bool mIsVertical;
    bool mIsDominant = false;
    s32 mDominantBorder = 8;
    s32 mDominantTimer = 0;
};

static_assert(sizeof(InputSeparator) == 0x18);
