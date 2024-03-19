#include "Project/Action/ActionAnimCtrl.h"

#include "Library/Base/String.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Resource/Resource.h"
#include "Library/Resource/ResourceHolder.h"

namespace al {
ActionAnimCtrl::ActionAnimCtrl(const LiveActor* actor) : mParentActor(actor), mAnimPartIndex(-1) {}

}
