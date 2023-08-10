#include "al/Library/Camera/SnapShotCameraCtrl.h"
#include "al/Library/Yaml/ByamlUtil.h"

namespace al {

// NON_MATCHING
void SnapShotCameraCtrl::load(ByamlIter const& iter) {
    ByamlIter param;
    if (!tryGetByamlIterByKey(&param, iter, "SnapShotParam"))
        return;
    if (tryGetByamlF32(&mParam->mMinFovyDegree, param, "MinFovyDegree"))
        mParam->gotMin = true;
    if (tryGetByamlF32(&mParam->mMinFovyDegree, param, "MinFovyDegree"))
        mParam->gotMax = true;
}

}  // namespace al
