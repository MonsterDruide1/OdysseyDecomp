#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

namespace al {
class IUseAudioKeeper;
class IUseLayoutAction;
class LayoutActor;
class MessageTagDataHolder;
class Nerve;
class ReplaceTagProcessorBase;

void startAction(IUseLayoutAction* layout, const char* actionName, const char* paneName);
s32 startActionAtRandomFrame(IUseLayoutAction* layout, const char* actionName,
                             const char* paneName);
void startFreezeAction(IUseLayoutAction* layout, const char* actionName, f32 frame,
                       const char* paneName);
void startFreezeActionEnd(IUseLayoutAction* layout, const char* actionName, const char* paneName);

f32 getActionFrameMax(const IUseLayoutAction* layout, const char* actionName, const char* paneName);

void startFreezeGaugeAction(IUseLayoutAction* layout, f32 value, f32 minFrame, f32 maxFrame,
                            const char* actionName, const char* paneName);

bool tryStartAction(IUseLayoutAction* layout, const char* actionName, const char* paneName);

bool isExistAction(const IUseLayoutAction* layout, const char* actionName, const char* paneName);
bool isActionEnd(const IUseLayoutAction* layout, const char* paneName);
bool isExistAction(const IUseLayoutAction* layout, const char* paneName);
bool isActionOneTime(const IUseLayoutAction* layout, const char* actionName, const char* paneName);

f32 getActionFrame(const IUseLayoutAction* layout, const char* paneName);
void setActionFrame(IUseLayoutAction* layout, f32 frame, const char* paneName);
f32 getActionFrameRate(const IUseLayoutAction* layout, const char* paneName);
void setActionFrameRate(IUseLayoutAction* layout, f32 frameRate, const char* paneName);

const char* getActionName(const IUseLayoutAction* layout, const char* paneName);

bool isActionPlaying(const IUseLayoutAction* layout, const char* actionName, const char* paneName);
bool isAnyActionPlaying(const IUseLayoutAction* layout, const char* paneName);

void setNerveAtActionEnd(LayoutActor* layout, const Nerve* nerve);

void startTextPaneAnim(LayoutActor*, const char16*, const MessageTagDataHolder*,
                       const ReplaceTagProcessorBase*);
void startTextPaneAnimWithAudioUser(LayoutActor*, const char16*, const MessageTagDataHolder*,
                                    const ReplaceTagProcessorBase*, const IUseAudioKeeper*);
void startAndSetTextPaneAnimStage(LayoutActor*, const char*, const char*,
                                  const MessageTagDataHolder*, const ReplaceTagProcessorBase*);
void startAndSetTextPaneAnimSystem(LayoutActor*, const char*, const char*,
                                   const MessageTagDataHolder*, const ReplaceTagProcessorBase*);
void endTextPaneAnim(LayoutActor*);
void skipTextPaneAnim(LayoutActor*);
void flushTextPaneAnim(LayoutActor*);
void changeNextPage(LayoutActor*, const MessageTagDataHolder*, const ReplaceTagProcessorBase*);
bool tryChangeNextPage(LayoutActor*, const MessageTagDataHolder*, const ReplaceTagProcessorBase*);
bool isExistNextPage(const LayoutActor*);
bool isEndTextPaneAnim(const LayoutActor*, bool);
const char16* getCurrentMessagePaneAnim(const LayoutActor*);
s32 calcCurrentMessageTextNum(const LayoutActor*);
s32 calcShowTextTime(s32);
bool tryStartTextAnim(LayoutActor*, const char16*);
bool tryStartTextTagVoice(LayoutActor*, const char16*, const IUseAudioKeeper*, const char*,
                          sead::FixedSafeString<64>*);
void startHitReaction(const LayoutActor*, const char*, const char*);
}  // namespace al
