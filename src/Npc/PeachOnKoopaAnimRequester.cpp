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
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->requestSuccess();
}

void requestStartPeachOnKoopaHitReaction2DIn(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->requestHitReaction2DIn();
}

void requestStartPeachOnKoopaHitReaction2DOut(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->requestHitReaction2DOut();
}

void requestStartPeachOnKoopaHitReactionBreakPillarBridge(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->requestHitReactionBreakPillarBridge();
}

void requestStartPeachOnKoopaHitReactionCrackGateHard(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->requestHitReactionCrackGateHard();
}

void requestStartPeachOnKoopaHitReactionBreakGateHard(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->requestHitReactionBreakGateHard();
}

void requestStartPeachOnKoopaHitReactionBreakPillar(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->requestHitReactionBreakPillar();
}

void requestStartPeachOnKoopaHitReactionBreakGateFinal(const al::IUseSceneObjHolder* holder) {
    if (al::isExistSceneObj(holder, SceneObjID_PeachOnKoopaAnimRequester))
        al::getSceneObj<PeachOnKoopaAnimRequester>(holder)->requestHitReactionBreakGateFinal();
}

}  // namespace rs
