#include "Library/LiveActor/ActorInitResourceData.h"

namespace al {

ActorInitResourceData::ActorInitResourceData(Resource* res) : mResource(res) {
    mResDataAnim = InitResourceDataAnim::tryCreate(res, nullptr);
    mResDataAction = InitResourceDataAction::tryCreate(res, mResDataAnim, nullptr);
}
}  // namespace al
