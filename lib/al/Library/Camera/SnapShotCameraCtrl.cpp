#include "Library/Camera/SnapShotCameraCtrl.h"

#include "Library/Yaml/ByamlUtil.h"

namespace al {

void SnapShotCameraCtrl::load(const ByamlIter& iter) {
    CameraParam* param = mParam;
    ByamlIter paramIter;
    if (!tryGetByamlIterByKey(&paramIter, iter, "SnapShotParam"))
        return;
    if (tryGetByamlF32(&param->mMinFovyDegree, paramIter, "MinFovyDegree"))
        param->mHasMin = true;
    if (tryGetByamlF32(&param->mMaxFovyDegree, paramIter, "MaxFovyDegree"))
        param->mHasMax = true;
}

}  // namespace al
