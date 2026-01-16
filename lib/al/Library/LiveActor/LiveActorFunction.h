#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
class SubActorKeeper;
}  // namespace al

namespace alSubActorFunction {

void trySyncAlive(al::SubActorKeeper* subActorKeeper);
void trySyncDead(al::SubActorKeeper* subActorKeeper);
void trySyncClippingStart(al::SubActorKeeper* subActorKeeper);
void trySyncClippingEnd(al::SubActorKeeper* subActorKeeper);
void trySyncShowModel(al::SubActorKeeper* subActorKeeper);
void trySyncHideModel(al::SubActorKeeper* subActorKeeper);
void trySyncModelAlphaMask(al::SubActorKeeper* subActorKeeper, f32 alpha);
al::LiveActor* findSubActor(const al::SubActorKeeper* subActorKeeper, const char* subActorName);

}  // namespace alSubActorFunction

namespace al {
bool isExistSubActorKeeper(const LiveActor* actor);
LiveActor* getSubActor(const LiveActor* actor, const char* subActorName);
LiveActor* tryGetSubActor(const LiveActor* actor, const char* subActorName);
LiveActor* getSubActor(const LiveActor* actor, s32 index);
s32 getSubActorNum(const LiveActor* actor);
void offSyncClippingSubActor(LiveActor* actor, const LiveActor* subActor);
void offSyncClippingSubActor(LiveActor* actor, const char* subActorName);
void offSyncClippingSubActorAll(LiveActor* actor);
void onSyncClippingSubActor(LiveActor* actor, const LiveActor* subActor);
void onSyncClippingSubActor(LiveActor* actor, const char* subActorName);
void onSyncClippingSubActorAll(LiveActor* actor);
void offSyncAppearSubActor(LiveActor* actor, const LiveActor* subActor);
void offSyncAppearSubActor(LiveActor* actor, const char* subActorName);
void offSyncAppearSubActorAll(LiveActor* actor);
void onSyncAppearSubActor(LiveActor* actor, const LiveActor* subActor);
void onSyncAppearSubActor(LiveActor* actor, const char* subActorName);
void onSyncAppearSubActorAll(LiveActor* actor);
void offSyncHideSubActor(LiveActor* actor, const LiveActor* subActor);
void offSyncHideSubActor(LiveActor* actor, const char* subActorName);
void offSyncHideSubActorAll(LiveActor* actor);
void onSyncHideSubActor(LiveActor* actor, const LiveActor* subActor);
void onSyncHideSubActor(LiveActor* actor, const char* subActorName);
void onSyncHideSubActorAll(LiveActor* actor);
bool isSyncHideSubActor(const LiveActor* actor, const LiveActor* subActor);
void offSyncAlphaMaskSubActor(LiveActor* actor, const LiveActor* subActor);
void offSyncAlphaMaskSubActor(LiveActor* actor, const char* subActorName);
void offSyncAlphaMaskSubActorAll(LiveActor* actor);
void onSyncAlphaMaskSubActor(LiveActor* actor, const LiveActor* subActor);
void onSyncAlphaMaskSubActor(LiveActor* actor, const char* subActorName);
void onSyncAlphaMaskSubActorAll(LiveActor* actor);
bool isSyncAlphaMaskSubActor(const LiveActor* actor, const LiveActor* subActor);
void startActionSubActor(const LiveActor* actor, const char* subActorName, const char* action);
bool isActionEndSubActor(const LiveActor* actor, const char* subActorName);
bool isActionPlayingSubActor(const LiveActor* actor, const char* subActorName, const char* action);
bool isActionOneTimeSubActor(const LiveActor* actor, const char* subActorName);
bool isActionOneTimeSubActor(const LiveActor* actor, const char* subActorName, const char* action);
bool tryStartActionSubActorAll(const LiveActor* actor, const char* action);
void makeActorDeadSubActorAll(const LiveActor* actor);
void tryValidateCollisionPartsSubActorAll(LiveActor* actor);
void tryInvalidateCollisionPartsSubActorAll(LiveActor* actor);

}  // namespace al
