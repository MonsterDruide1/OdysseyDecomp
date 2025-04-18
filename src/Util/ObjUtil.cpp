#include "Util/ObjUtil.h"

#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"

#include "Player/PlayerWallActionHistory.h"

namespace rs {

bool judgeEnableWallKeepHistory(const al::LiveActor* player, const PlayerWallActionHistory* history,
                                const sead::Vector3f& calcPos, const sead::Vector3f& wallNormal,
                                f32 unk, bool unk2) {
    sead::Vector3f up = -al::getGravity(player);

    if (!history->isLeaveStored() || !(up.dot(calcPos - history->getLeaveWallPosition()) > -unk)) {
        if (!history->isJumpStored())
            return true;

        sead::Vector3f diffToLastJumpPos;
        sead::Vector3f horizontalDiff = {0.0f, 0.0f, 0.0f};
        diffToLastJumpPos.x = (calcPos - history->getJumpWallPosition()).x;
        diffToLastJumpPos.y = (calcPos - history->getJumpWallPosition()).y;
        diffToLastJumpPos.z = (calcPos - history->getJumpWallPosition()).z;
        al::verticalizeVec(&horizontalDiff, up, diffToLastJumpPos);

        f32 cosAngleOfWalls = wallNormal.dot(history->getJumpWallNormal());
        if (cosAngleOfWalls > -0.34202f) {  // cos(110°), smaller => larger angle
            f32 v21 = 0.0f;
            if (unk2) {
                f32 horizontalDist = horizontalDiff.length();
                f32 clamp;
                if (cosAngleOfWalls > 0.087156f)  // cos(85°)
                    clamp = sead::Mathf::clamp((750.0f - horizontalDist) / 750.0f, -1.0f, 1.0f);
                else
                    clamp = sead::Mathf::clamp((1000.0f - horizontalDist) / 500.0f, -1.0f, 1.0f);
                v21 = ((clamp - 1.0f) * 1500.0f) * 0.5f;
            }
            if (up.dot(diffToLastJumpPos) <= v21)
                return false;
            return true;
        } else {
            if (unk2) {
                f32 horizontalDist = horizontalDiff.length();
                if (horizontalDist > 1500.0f &&
                    (up.dot(diffToLastJumpPos) >= 1500.0f - horizontalDist))
                    return false;
            }
            return true;
        }
    }
    return false;
}

}  // namespace rs
