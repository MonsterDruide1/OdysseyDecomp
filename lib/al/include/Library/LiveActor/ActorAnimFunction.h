#pragma once

namespace al {
class LiveActor;

void startSklAnim(LiveActor*, const char*);
void startSklAnimBlend(LiveActor*, const char*, const char*, const char*, const char*, const char*,
                       const char*);
void startSklAnimInterpole(LiveActor*, const char*, const char*);
void startSklAnimBlendInterpole(LiveActor*, const char*, const char*, const char*, const char*,
                                const char*, const char*, const char*);
void startMtpAnim(const LiveActor*, const char*);
void startMclAnim(const LiveActor*, const char*);
void startMtsAnim(const LiveActor*, const char*);
void startVisAnimForAction(const LiveActor*, const char*);
bool isSklAnimExist(const LiveActor*, const char*);
bool isMtpAnimExist(const LiveActor*, const char*);
bool isMclAnimExist(const LiveActor*, const char*);
bool isMtsAnimExist(const LiveActor*, const char*);
bool isVisAnimExist(const LiveActor*, const char*);
bool isSklAnimPlaying(const LiveActor*, const char*, int);
bool isMtpAnimPlaying(const LiveActor*, const char*);
bool isMclAnimPlaying(const LiveActor*, const char*);
bool isMtsAnimPlaying(const LiveActor*, const char*);
bool isVisAnimPlaying(const LiveActor*, const char*);
}  // namespace al
