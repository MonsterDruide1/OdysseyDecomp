#include "Npc/PeachOnKoopaAnimRequester.h"

#include "Library/Scene/SceneObjUtil.h"

PeachOnKoopaAnimRequester::PeachOnKoopaAnimRequester() = default;

void PeachOnKoopaAnimRequester::resetRequest() {
    mIsRequestSuccess = false;
    mIsRequestHitReaction2DIn = false;
    mIsRequestHitReaction2DOut = false;
    mIsRequestHitReactionBreakPillarBridge = false;
    mIsRequestHitReactionCrackGateHard = false;
    mIsRequestHitReactionBreakGateHard = false;
    mIsRequestHitReactionBreakPillar = false;
    mIsRequestHitReactionBreakGateFinal = false;
}

const char* PeachOnKoopaAnimRequester::getSceneObjName() const {
    return "肩乗りピーチへのアニメーションリクエスト";
}

namespace rs {

PeachOnKoopaAnimRequester* initPeachOnKoopaAnimRequester(const al::IUseSceneObjHolder* holder) {
    PeachOnKoopaAnimRequester* requester = new PeachOnKoopaAnimRequester();
    al::setSceneObj(holder, requester, SceneObjID_PeachOnKoopaAnimRequester);
    return requester;
}

void requestStartPeachOnKoopaAnimSuccess(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->mIsRequestSuccess = true;
}

void requestStartPeachOnKoopaHitReaction2DIn(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->mIsRequestHitReaction2DIn = true;
}

void requestStartPeachOnKoopaHitReaction2DOut(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->mIsRequestHitReaction2DOut = true;
}

void requestStartPeachOnKoopaHitReactionBreakPillarBridge(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->mIsRequestHitReactionBreakPillarBridge =
            true;
}

void requestStartPeachOnKoopaHitReactionCrackGateHard(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->mIsRequestHitReactionCrackGateHard =
            true;
}

void requestStartPeachOnKoopaHitReactionBreakGateHard(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->mIsRequestHitReactionBreakGateHard =
            true;
}

void requestStartPeachOnKoopaHitReactionBreakPillar(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->mIsRequestHitReactionBreakPillar = true;
}

void requestStartPeachOnKoopaHitReactionBreakGateFinal(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->mIsRequestHitReactionBreakGateFinal =
            true;
}

}  // namespace rs
