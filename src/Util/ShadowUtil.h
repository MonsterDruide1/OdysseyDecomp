#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
class ActorInitInfo;
}  // namespace al

namespace rs {

f32 setShadowDropLength(al::LiveActor*, const al::ActorInitInfo&, const char*);

}  // namespace rs
