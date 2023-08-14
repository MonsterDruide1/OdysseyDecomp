#include "Library/Camera/CameraPoser.h"

#include "Library/Camera/CameraPoserFunction.h"
#include "Library/Camera/SnapShotCameraCtrl.h"

namespace al {
void CameraPoser::startSnapShotModeCore() {
    if (mSnapshotCtrl)
        mSnapshotCtrl->start(mFovyDegree);
    startSnapShotMode();
}

void CameraPoser::endSnapShotModeCore() {
    endSnapShotMode();
}

f32 CameraPoser::getFovyDegree() const {
    if (alCameraPoserFunction::isSnapShotMode(this) && mSnapshotCtrl)
        return mSnapshotCtrl->getFovyDegree();
    return mFovyDegree;
}
}  // namespace al
