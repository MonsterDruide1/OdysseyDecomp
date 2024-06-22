#include "Util/ObjUtil.h"

#include "Library/LiveActor/ActorPoseKeeper.h"

#include "Library/Math/MathAngleUtil.h"
#include "Player/PlayerWallActionHistory.h"

namespace rs {

bool judgeEnableWallKeepHistory(const al::LiveActor* player, const PlayerWallActionHistory* history,
                                const sead::Vector3f& calcPos, const sead::Vector3f& wallNormal,
                                f32 unk, bool unk2) {
    sead::Vector3f up = -al::getGravity(player);

    if (history->isLeaveStored() && up.dot(calcPos - history->getLeaveWallPosition()) > -unk)
        return false;

    if (!history->isJumpStored())
        return true;

    sead::Vector3f horizontalDiff = {0.0f, 0.0f, 0.0f};
    sead::Vector3f diffToLastJumpPos;
    diffToLastJumpPos.x = (calcPos - history->getJumpWallPosition()).x;
    diffToLastJumpPos.y = (calcPos - history->getJumpWallPosition()).y;
    diffToLastJumpPos.z = (calcPos - history->getJumpWallPosition()).z;
    al::verticalizeVec(&horizontalDiff, up, diffToLastJumpPos);

    f32 cosAngleOfWalls = wallNormal.dot(history->getJumpWallNormal());
    if (cosAngleOfWalls > -0.34202f) {  // cos(110°), smaller => larger angle
        f32 v21 = 0.0f;
        if (unk2) {
            f32 horizontalDist = horizontalDiff.length();
            if (cosAngleOfWalls <= 0.087156f) {  // cos(85°)
                v21 = ((sead::Mathf::clamp((1000.0f - horizontalDist) / 500.0f, -1.0f, 1.0f) - 1.0f) * 1500.0f) * 0.5f;
            } else {
                v21 = ((sead::Mathf::clamp((750.0f - horizontalDist) / 750.0f, -1.0f, 1.0f) - 1.0f) * 1500.0f) * 0.5f;
            }
        }
        return up.dot(diffToLastJumpPos) <= v21;
    }
    if (!unk2)
        return true;
    f32 horizontalDist = horizontalDiff.length();
    return horizontalDist <= 1500.0f || up.dot(diffToLastJumpPos) <= 1500.0f - horizontalDist;
}

}  // namespace rs
