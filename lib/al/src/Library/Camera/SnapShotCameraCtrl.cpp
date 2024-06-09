#include "Library/Camera/SnapShotCameraCtrl.h"

#include "Library/Yaml/ByamlUtil.h"

namespace al {

// NON_MATCHING
void SnapShotCameraCtrl::load(const ByamlIter& iter) {
    ByamlIter param;
    if (!tryGetByamlIterByKey(&param, iter, "SnapShotParam"))
        return;
    if (tryGetByamlF32(&mParam->mMinFovyDegree, param, "MinFovyDegree"))
        mParam->mHasMin = true;
    if (tryGetByamlF32(&mParam->mMinFovyDegree, param, "MinFovyDegree"))
        mParam->mHasMax = true;
}

}  // namespace al
