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

namespace rs {

PeachOnKoopaAnimRequester* initPeachOnKoopaAnimRequester(const al::IUseSceneObjHolder* holder) {
    PeachOnKoopaAnimRequester* requester = new PeachOnKoopaAnimRequester();
    al::setSceneObj(holder, requester, SceneObjID_PeachOnKoopaAnimRequester);
    return requester;
}

void requestStartPeachOnKoopaAnimSuccess(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->setRequestSuccess(true);
}

void requestStartPeachOnKoopaHitReaction2DIn(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->setRequestHitReaction2DIn(true);
}

void requestStartPeachOnKoopaHitReaction2DOut(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->setRequestHitReaction2DOut(true);
}

void requestStartPeachOnKoopaHitReactionBreakPillarBridge(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->setRequestHitReactionBreakPillarBridge(
            true);
}

void requestStartPeachOnKoopaHitReactionCrackGateHard(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->setRequestHitReactionCrackGateHard(
            true);
}

void requestStartPeachOnKoopaHitReactionBreakGateHard(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->setRequestHitReactionBreakGateHard(
            true);
}

void requestStartPeachOnKoopaHitReactionBreakPillar(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->setRequestHitReactionBreakPillar(true);
}

void requestStartPeachOnKoopaHitReactionBreakGateFinal(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->setRequestHitReactionBreakGateFinal(
            true);
}

}  // namespace rs
