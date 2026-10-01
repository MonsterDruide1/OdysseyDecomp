#pragma once

#include <prim/seadSafeString.h>

namespace al {
class LiveActor;
}

class TalkNpcParam;

namespace rs {
TalkNpcParam* initTalkNpcParam(al::LiveActor*, const char*);
void startNpcAction(al::LiveActor*, const char*);
void makeNpcActionName(sead::BufferedSafeStringBase<char>*, const al::LiveActor*, const char*);
bool tryStartNpcActionIfNotPlaying(al::LiveActor*, const char*);
bool isExistNpcAction(const al::LiveActor*, const char*);
bool isPlayingNpcAction(const al::LiveActor*, const char*);
bool isOneTimeNpcAction(const al::LiveActor*, const char*);
bool isExistTalkNpcParamHolder(const al::LiveActor*);
bool isInvalidNpcScare(const TalkNpcParam*);
bool checkEnableStartEventAndCancelReaction(al::LiveActor*, const TalkNpcParam*);
}  // namespace rs
