#include "Library/LiveActor/ActorAreaFunction.h"

#include "Library/Area/AreaInitInfo.h"
#include "Library/Area/AreaObj.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorSceneInfo.h"

namespace al {

AreaObj* createAreaObj(const ActorInitInfo& actorInitInfo, const char* name) {
    AreaInitInfo areaInitInfo;
    areaInitInfo.set(*actorInitInfo.placementInfo, actorInitInfo.stageSwitchDirector,
                     actorInitInfo.actorSceneInfo.sceneObjHolder);
    AreaObj* areaObj = new AreaObj(name);
    areaObj->init(areaInitInfo);
    return areaObj;
}

}  // namespace al
