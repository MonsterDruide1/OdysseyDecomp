#include "Library/Camera/CameraPoserSceneInfo.h"

namespace al {

void CameraPoserSceneInfo::init(AreaObjDirector* areaObj, CollisionDirector* collision,
                                const AudioDirector* audio) {
    areaObjDirector = areaObj;
    collisionDirector = collision;
    audioDirector = audio;
}

}  // namespace al
