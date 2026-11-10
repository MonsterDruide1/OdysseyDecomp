#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}  // namespace al
class TalkNpcParam;

namespace rs {
TalkNpcParam* initTalkNpcParam(al::LiveActor* actor, const char* name);
}  // namespace rs
