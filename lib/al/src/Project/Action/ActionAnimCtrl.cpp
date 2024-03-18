#include "Project/Action/ActionAnimCtrl.h"

#include "Library/Base/String.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Resource/Resource.h"
#include "Library/Resource/ResourceHolder.h"

namespace al {

ActionAnimCtrl::ActionAnimCtrl(const LiveActor* actor) : mParentActor(actor), mAnimPartIndex(-1) {}

ActionAnimCtrl* ActionAnimCtrl::tryCreate(LiveActor* actor, const ActorResource* actor_resource, const char* param_3, const char* param_4){
  Resource* resource;
  ActionAnimCtrl* new_this;
  sead::BufferedSafeString* fileName;
  
  if (actor->getModelKeeper() == nullptr) {
    new_this = nullptr;
  }
  else {

    resource = actor_resource->mAnimRes;
    if (actor_resource->mHasAnimData) {
      resource = actor_resource->mActionRes;
    }
    if (!tryGetActorInitFileName(fileName, actor, "ActionAnimCtrl", param_4)) {
      createFileNameBySuffix(fileName, "ActionAnimCtrl", param_4);
    }

    new_this = nullptr;
    if (isExistResourceYaml(resource,actor->mActorName,nullptr)) {
      new_this = new ActionAnimCtrl(actor);
      //init(new_this,actor_resource->mName,param_3);
    }
  }
  return new_this;
}

}