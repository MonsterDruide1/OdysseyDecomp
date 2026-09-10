#pragma once

#include <prim/seadSafeString.h>

namespace al {
class LiveActor;
}  // namespace al

class TalkNpcParam;

namespace rs {
TalkNpcParam* initTalkNpcParam(al::LiveActor* actor, const char*);
void startNpcAction(al::LiveActor* actor, const char* action);
s32 makeNpcActionName(sead::BufferedSafeStringBase<char>* outName, const al::LiveActor* actor,
                      const char* action);
bool tryStartNpcActionIfNotPlaying(al::LiveActor* actor, const char* action);
bool isExistNpcAction(const al::LiveActor* actor, const char* action);
bool isPlayingNpcAction(const al::LiveActor* actor, const char* action);
bool isOneTimeNpcAction(const al::LiveActor* actor, const char* action);
bool isExistTalkNpcParamHolder(const al::LiveActor* actor);
bool isInvalidNpcScare(const TalkNpcParam*);
bool checkEnableStartEventAndCancelReaction(al::LiveActor* actor, const TalkNpcParam*);
}  // namespace rs
