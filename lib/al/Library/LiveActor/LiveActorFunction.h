#pragma once

#include "al/Library/LiveActor/LiveActor.h"

namespace al {
void isExistSubActorKeeper(const LiveActor*);
void getSubActor(const LiveActor*, const char*);
void tryGetSubActor(const LiveActor*, const char*);
void getSubActor(const LiveActor*, int);
void getSubActorNum(const LiveActor*);
void offSyncClippingSubActor(LiveActor*, const LiveActor*);
void offSyncClippingSubActor(LiveActor*, const char*);
void offSyncClippingSubActorAll(LiveActor*);
void onSyncClippingSubActor(LiveActor*, const LiveActor*);
void onSyncClippingSubActor(LiveActor*, const char*);
void onSyncClippingSubActorAll(LiveActor*);
void offSyncAppearSubActor(LiveActor*, const LiveActor*);
void offSyncAppearSubActor(LiveActor*, const char*);
void offSyncAppearSubActorAll(LiveActor*);
void onSyncAppearSubActor(LiveActor*, const LiveActor*);
void onSyncAppearSubActor(LiveActor*, const char*);
void onSyncAppearSubActorAll(LiveActor*);
void offSyncHideSubActor(LiveActor*, const LiveActor*);
void offSyncHideSubActor(LiveActor*, const char*);
void offSyncHideSubActorAll(LiveActor*);
void onSyncHideSubActor(LiveActor*, const LiveActor*);
void onSyncHideSubActor(LiveActor*, const char*);
void onSyncHideSubActorAll(LiveActor*);
void isSyncHideSubActor(const LiveActor*, const LiveActor*);
void offSyncAlphaMaskSubActor(LiveActor*, const LiveActor*);
void offSyncAlphaMaskSubActor(LiveActor*, const char*);
void offSyncAlphaMaskSubActorAll(LiveActor*);
void onSyncAlphaMaskSubActor(LiveActor*, const LiveActor*);
void onSyncAlphaMaskSubActor(LiveActor*, const char*);
void onSyncAlphaMaskSubActorAll(LiveActor*);
void isSyncAlphaMaskSubActor(const LiveActor*, const LiveActor*);
void startActionSubActor(const LiveActor*, const char*, const char*);
void isActionEndSubActor(const LiveActor*, const char*);
void isActionPlayingSubActor(const LiveActor*, const char*, const char*);
void isActionOneTimeSubActor(const LiveActor*, const char*);
void isActionOneTimeSubActor(const LiveActor*, const char*, const char*);
void tryStartActionSubActorAll(const LiveActor*, const char*);
void makeActorDeadSubActorAll(const LiveActor*);
void tryValidateCollisionPartsSubActorAll(LiveActor*);
void tryInvalidateCollisionPartsSubActorAll(LiveActor*);
}  // namespace al

namespace alSubActorFunction {
bool trySyncAlive(al::SubActorKeeper*);
bool trySyncDead(al::SubActorKeeper*);
bool trySyncClippingStart(al::SubActorKeeper*);
bool trySyncClippingEnd(al::SubActorKeeper*);
bool trySyncShowModel(al::SubActorKeeper*);
bool trySyncHideModel(al::SubActorKeeper*);
bool trySyncModelAlphaMask(al::SubActorKeeper*, float);
void findSubActor(const al::SubActorKeeper*, const char*);
}  // namespace alSubActorFunction
