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

void startAction(IUseLayoutAction* layout, const char* actionName, const char* paneName = nullptr);
s32 startActionAtRandomFrame(IUseLayoutAction* layout, const char* actionName,
                             const char* paneName = nullptr);
void startFreezeAction(IUseLayoutAction* layout, const char* actionName, f32 frame,
                       const char* paneName = nullptr);
void startFreezeActionEnd(IUseLayoutAction* layout, const char* actionName,
                          const char* paneName = nullptr);

f32 getActionFrameMax(const IUseLayoutAction* layout, const char* actionName,
                      const char* paneName = nullptr);

void startFreezeGaugeAction(IUseLayoutAction* layout, f32 value, f32 minFrame, f32 maxFrame,
                            const char* actionName, const char* paneName = nullptr);

bool tryStartAction(IUseLayoutAction* layout, const char* actionName,
                    const char* paneName = nullptr);

bool isExistAction(const IUseLayoutAction* layout, const char* actionName, const char* paneName);
bool isActionEnd(const IUseLayoutAction* layout, const char* paneName = nullptr);
bool isExistAction(const IUseLayoutAction* layout, const char* paneName = nullptr);
bool isActionOneTime(const IUseLayoutAction* layout, const char* actionName,
                     const char* paneName = nullptr);

f32 getActionFrame(const IUseLayoutAction* layout, const char* paneName = nullptr);
void setActionFrame(IUseLayoutAction* layout, f32 frame, const char* paneName = nullptr);
f32 getActionFrameRate(const IUseLayoutAction* layout, const char* paneName = nullptr);
void setActionFrameRate(IUseLayoutAction* layout, f32 frameRate, const char* paneName = nullptr);

const char* getActionName(const IUseLayoutAction* layout, const char* paneName = nullptr);

bool isActionPlaying(const IUseLayoutAction* layout, const char* actionName,
                     const char* paneName = nullptr);
bool isAnyActionPlaying(const IUseLayoutAction* layout, const char* paneName = nullptr);

void setNerveAtActionEnd(LayoutActor* layout, const Nerve* nerve);

void startTextPaneAnim(LayoutActor* layout, const char16*, const MessageTagDataHolder*,
                       const ReplaceTagProcessorBase*);
void startTextPaneAnimWithAudioUser(LayoutActor* layout, const char16*, const MessageTagDataHolder*,
                                    const ReplaceTagProcessorBase*, const IUseAudioKeeper*);
void startAndSetTextPaneAnimStage(LayoutActor* layout, const char*, const char*,
                                  const MessageTagDataHolder*, const ReplaceTagProcessorBase*);
void startAndSetTextPaneAnimSystem(LayoutActor* layout, const char*, const char*,
                                   const MessageTagDataHolder*, const ReplaceTagProcessorBase*);
void endTextPaneAnim(LayoutActor* layout);
void skipTextPaneAnim(LayoutActor* layout);
void flushTextPaneAnim(LayoutActor* layout);
void changeNextPage(LayoutActor* layout, const MessageTagDataHolder*,
                    const ReplaceTagProcessorBase*);
bool tryChangeNextPage(LayoutActor* layout, const MessageTagDataHolder*,
                       const ReplaceTagProcessorBase*);
bool isExistNextPage(const LayoutActor* layout);
bool isEndTextPaneAnim(const LayoutActor* layout, bool);
const char16* getCurrentMessagePaneAnim(const LayoutActor* layout);
s32 calcCurrentMessageTextNum(const LayoutActor* layout);
s32 calcShowTextTime(s32);
bool tryStartTextAnim(LayoutActor* layout, const char16*);
bool tryStartTextTagVoice(LayoutActor* layout, const char16*, const IUseAudioKeeper*, const char*,
                          sead::FixedSafeString<64>*);
void startHitReaction(const LayoutActor* layout, const char* hitReactionName,
                      const char* paneName = nullptr);
}  // namespace al
