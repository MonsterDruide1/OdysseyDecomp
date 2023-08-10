#include "al/Library/Camera/CameraPoser.h"

#include "al/Library/Camera/CameraPoserFunction.h"
#include "al/Library/Camera/SnapShotCameraCtrl.h"

namespace al {
void CameraPoser::startSnapShotModeCore() {
    if (mSnapshotCtrl)
        mSnapshotCtrl->start(mFovyDegree);
    startSnapShotMode();
}

void CameraPoser::endSnapShotModeCore() {
    endSnapShotMode();
}

float CameraPoser::getFovyDegree() const {
    if (alCameraPoserFunction::isSnapShotMode(this) && mSnapshotCtrl)
        return mSnapshotCtrl->getFovyDegree();
    return mFovyDegree;
}
}  // namespace al
