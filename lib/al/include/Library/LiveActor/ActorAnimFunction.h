#pragma once

namespace al {
class LiveActor;

static void startSklAnim(LiveActor*, const char*);
static void startSklAnimBlend(LiveActor*, const char*, const char*, const char*, const char*,
                              const char*, const char*);
static void startSklAnimInterpole(LiveActor*, const char*, const char*);
static void startSklAnimBlendInterpole(LiveActor*, const char*, const char*, const char*,
                                       const char*, const char*, const char*, const char*);
static void startMtpAnim(const LiveActor*, const char*);
static void startMclAnim(const LiveActor*, const char*);
static void startMtsAnim(const LiveActor*, const char*);
static void startVisAnimForAction(const LiveActor*, const char*);

static bool isSklAnimExist(const LiveActor*, const char*);
static bool isMtpAnimExist(const LiveActor*, const char*);
static bool isMclAnimExist(const LiveActor*, const char*);
static bool isMtsAnimExist(const LiveActor*, const char*);
static bool isVisAnimExist(const LiveActor*, const char*);
static bool isSklAnimPlaying(const LiveActor*, const char*, int);
static bool isMtpAnimPlaying(const LiveActor*, const char*);
static bool isMclAnimPlaying(const LiveActor*, const char*);
static bool isMtsAnimPlaying(const LiveActor*, const char*);
static bool isVisAnimPlaying(const LiveActor*, const char*);
}  // namespace al
