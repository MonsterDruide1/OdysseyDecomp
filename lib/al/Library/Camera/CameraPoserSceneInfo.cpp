#include "Library/Camera/CameraPoserSceneInfo.h"

#include "Library/Obj/CameraRailHolder.h"

namespace al {

CameraPoserSceneInfo::CameraPoserSceneInfo() {
    railHolders = new CameraRailHolder*[16];
}

void CameraPoserSceneInfo::registerCameraRailHolder(CameraRailHolder* railHolder) {
    railHolders[railHolderNum] = railHolder;
    railHolderNum++;
}

void CameraPoserSceneInfo::init(AreaObjDirector* areaObj, CollisionDirector* collision,
                                const AudioDirector* audio) {
    areaObjDirector = areaObj;
    collisionDirector = collision;
    audioDirector = audio;
}

}  // namespace al
