#include "Library/Camera/SnapShotCameraCtrl.h"

#include "Library/Yaml/ByamlUtil.h"

namespace al {

void SnapShotCameraCtrl::load(const ByamlIter& iter) {
    CameraParam* param = mParam;
    ByamlIter paramIter;
    if (!tryGetByamlIterByKey(&paramIter, iter, "SnapShotParam"))
        return;
    if (tryGetByamlF32(&param->minFovyDegree, paramIter, "MinFovyDegree"))
        param->hasMin = true;
    if (tryGetByamlF32(&param->maxFovyDegree, paramIter, "MaxFovyDegree"))
        param->hasMax = true;
}

}  // namespace al
