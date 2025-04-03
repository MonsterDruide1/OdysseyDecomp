#include "Library/Layout/LayoutActionFunction.h"

#include "Library/Base/StringUtil.h"
#include "Library/Layout/IUseLayoutAction.h"
#include "Library/Layout/LayoutActionKeeper.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutPaneGroup.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace al {

LayoutPaneGroup* getLayoutPaneGroup(const IUseLayoutAction* layout, const char* paneName) {
    return layout->getLayoutActionKeeper()->getLayoutPaneGroup(paneName);
}

void startAction(IUseLayoutAction* layout, const char* actionName, const char* paneName) {
    layout->getLayoutActionKeeper()->startAction(actionName, paneName);
}

s32 startActionAtRandomFrame(IUseLayoutAction* layout, const char* actionName,
                             const char* paneName) {
    startAction(layout, actionName, paneName);
    LayoutPaneGroup* paneGroup = getLayoutPaneGroup(layout, paneName);
    f32 animFrameMax = paneGroup->getAnimFrameMax(actionName);
    f32 randomFrame = getRandom(0.0f, animFrameMax);
    paneGroup->setAnimFrame((s32)randomFrame);
    return randomFrame;
}

void startFreezeAction(IUseLayoutAction* layout, const char* actionName, f32 frame,
                       const char* paneName) {
    startAction(layout, actionName, paneName);
    LayoutPaneGroup* paneGroup = getLayoutPaneGroup(layout, paneName);
    paneGroup->setAnimFrame(frame);
    paneGroup->setAnimFrameRate(0.0f);
}

void startFreezeActionEnd(IUseLayoutAction* layout, const char* actionName, const char* paneName) {
    f32 animFrameEnd = getActionFrameMax(layout, actionName, paneName);
    startFreezeAction(layout, actionName, animFrameEnd, paneName);
}

f32 getActionFrameMax(const IUseLayoutAction* layout, const char* actionName,
                      const char* paneName) {
    return getLayoutPaneGroup(layout, paneName)->getAnimFrameMax(actionName);
}

void startFreezeGaugeAction(IUseLayoutAction* layout, f32 value, f32 minFrame, f32 maxFrame,
                            const char* actionName, const char* paneName) {
    f32 frame =
        calcRate01(value, minFrame, maxFrame) * getActionFrameMax(layout, actionName, paneName);
    startFreezeAction(layout, actionName, frame, paneName);
}

bool tryStartAction(IUseLayoutAction* layout, const char* actionName, const char* paneName) {
    if (!isExistAction(layout, actionName, paneName))
        return false;

    startAction(layout, actionName, paneName);
    return true;
}

bool isExistAction(const IUseLayoutAction* layout, const char* actionName, const char* paneName) {
    return isExistAction(layout, paneName) &&
           getLayoutPaneGroup(layout, paneName)->isAnimExist(actionName);
}

bool isActionEnd(const IUseLayoutAction* layout, const char* paneName) {
    LayoutPaneGroup* paneGroup = getLayoutPaneGroup(layout, paneName);
    if (paneGroup && paneGroup->isAnimPlaying() && paneGroup->isAnimOneTime())
        return paneGroup->isAnimEnd();
    return true;
}

bool isExistAction(const IUseLayoutAction* layout, const char* paneName) {
    return layout->getLayoutActionKeeper()->getLayoutPaneGroup(paneName) != nullptr;
}

bool isActionOneTime(const IUseLayoutAction* layout, const char* actionName, const char* paneName) {
    return getLayoutPaneGroup(layout, paneName)->isAnimOneTime(actionName);
}

f32 getActionFrame(const IUseLayoutAction* layout, const char* paneName) {
    return getLayoutPaneGroup(layout, paneName)->getAnimFrame();
}

void setActionFrame(IUseLayoutAction* layout, f32 frame, const char* paneName) {
    getLayoutPaneGroup(layout, paneName)->setAnimFrame(frame);
}

f32 getActionFrameRate(const IUseLayoutAction* layout, const char* paneName) {
    return getLayoutPaneGroup(layout, paneName)->getAnimFrameRate();
}

void setActionFrameRate(IUseLayoutAction* layout, f32 frameRate, const char* paneName) {
    getLayoutPaneGroup(layout, paneName)->setAnimFrameRate(frameRate);
}

const char* getActionName(const IUseLayoutAction* layout, const char* paneName) {
    return getLayoutPaneGroup(layout, paneName)->getPlayingAnimName();
}

bool isActionPlaying(const IUseLayoutAction* layout, const char* actionName, const char* paneName) {
    return isEqualString(getLayoutPaneGroup(layout, paneName)->getPlayingAnimName(), actionName);
}

bool isAnyActionPlaying(const IUseLayoutAction* layout, const char* paneName) {
    return getLayoutPaneGroup(layout, paneName)->isAnimPlaying();
}

void setNerveAtActionEnd(LayoutActor* layout, const Nerve* nerve) {
    if (isActionEnd(layout, nullptr))
        setNerve(layout, nerve);
}

}  // namespace al
