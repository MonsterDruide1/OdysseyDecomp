#pragma once

#include <basis/seadTypes.h>

namespace al {
class IUseLayoutAction;
class LayoutActor;
class Nerve;

void startAction(IUseLayoutAction* layout, const char* actionName, const char* paneName);
void startFreezeAction(IUseLayoutAction* layout, const char* actionName, f32 frame,
                       const char* paneName);
void startFreezeActionEnd(IUseLayoutAction* layout, const char* actionName, const char* paneName);
void startActionAtRandomFrame(IUseLayoutAction* layout, const char* actionName,
                              const char* paneName);
void startFreezeGaugeAction(IUseLayoutAction* layout, f32 value, f32 minFrame, f32 maxFrame,
                            const char* actionName, const char* paneName);

bool tryStartAction(IUseLayoutAction* layout, const char* actionName, const char* paneName);

f32 getActionFrame(const IUseLayoutAction* layout, const char* paneName);
f32 getActionFrameRate(const IUseLayoutAction* layout, const char* paneName);
f32 getActionFrameMax(const IUseLayoutAction* layout, const char* actionName, const char* paneName);

void setActionFrameRate(const IUseLayoutAction* layout, f32 frameRate, const char* paneName);
void setActionFrame(const IUseLayoutAction* layout, f32 frame, const char* paneName);

bool isActionOneTime(const IUseLayoutAction* layout, const char* actionName, const char* paneName);
bool isActionPlaying(const IUseLayoutAction* layout, const char* actionName, const char* paneName);
bool isAnyActionPlaying(const IUseLayoutAction* layout, const char* paneName);
bool isActionEnd(const IUseLayoutAction* layout, const char* paneName);

bool isExistAction(const IUseLayoutAction* layout, const char* actionName);
bool isExistAction(const IUseLayoutAction* layout, const char* actionName, const char* paneName);

void setNerveAtActionEnd(LayoutActor*, const Nerve* nerve);

void startHitReaction(const LayoutActor*, const char*, const char*);
}  // namespace al
