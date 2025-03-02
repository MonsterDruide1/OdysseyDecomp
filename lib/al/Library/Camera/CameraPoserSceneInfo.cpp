#include "Library/Camera/CameraPoserSceneInfo.h"

namespace al {

void CameraPoserSceneInfo::init(AreaObjDirector* areaObj, CollisionDirector* collision,
                                const AudioDirector* audio) {
    mAreaObjDirector = areaObj;
    mCollisionDirector = collision;
    mAudioDirector = audio;
}

}  // namespace al

