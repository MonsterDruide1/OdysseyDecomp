#include "Boss/BombTail/BombTailTailPartsModelUpdater.h"

void BombTailTailPartsModelUpdater::preCalcAnim() {
    updatePose();
}

void BombTailTailPartsModelUpdater::postEndClipped() {
    updatePose();
}
