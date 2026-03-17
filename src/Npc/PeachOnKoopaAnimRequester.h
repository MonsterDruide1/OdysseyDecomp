#pragma once

#include <basis/seadTypes.h>

#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
class IUseSceneObjHolder;
}

class PeachOnKoopaAnimRequester : public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_PeachOnKoopaAnimRequester;

    PeachOnKoopaAnimRequester();

    void resetRequest();

    const char* getSceneObjName() const override {
        return "肩乗りピーチへのアニメーションリクエスト";
    }

    void requestSuccess() { mIsRequestSuccess = true; }

    void requestHitReaction2DIn() { mIsRequestHitReaction2DIn = true; }

    void requestHitReaction2DOut() { mIsRequestHitReaction2DOut = true; }

    void requestHitReactionBreakPillarBridge() { mIsRequestHitReactionBreakPillarBridge = true; }

    void requestHitReactionCrackGateHard() { mIsRequestHitReactionCrackGateHard = true; }

    void requestHitReactionBreakGateHard() { mIsRequestHitReactionBreakGateHard = true; }

    void requestHitReactionBreakPillar() { mIsRequestHitReactionBreakPillar = true; }

    void requestHitReactionBreakGateFinal() { mIsRequestHitReactionBreakGateFinal = true; }

private:
    bool mIsRequestSuccess = false;
    bool mIsRequestHitReaction2DIn = false;
    bool mIsRequestHitReaction2DOut = false;
    bool mIsRequestHitReactionBreakPillarBridge = false;
    bool mIsRequestHitReactionCrackGateHard = false;
    bool mIsRequestHitReactionBreakGateHard = false;
    bool mIsRequestHitReactionBreakPillar = false;
    bool mIsRequestHitReactionBreakGateFinal = false;
};

static_assert(sizeof(PeachOnKoopaAnimRequester) == 0x10);

namespace rs {

PeachOnKoopaAnimRequester* initPeachOnKoopaAnimRequester(const al::IUseSceneObjHolder* holder);
void requestStartPeachOnKoopaAnimSuccess(const al::IUseSceneObjHolder* holder);
void requestStartPeachOnKoopaHitReaction2DIn(const al::IUseSceneObjHolder* holder);
void requestStartPeachOnKoopaHitReaction2DOut(const al::IUseSceneObjHolder* holder);
void requestStartPeachOnKoopaHitReactionBreakPillarBridge(const al::IUseSceneObjHolder* holder);
void requestStartPeachOnKoopaHitReactionCrackGateHard(const al::IUseSceneObjHolder* holder);
void requestStartPeachOnKoopaHitReactionBreakGateHard(const al::IUseSceneObjHolder* holder);
void requestStartPeachOnKoopaHitReactionBreakPillar(const al::IUseSceneObjHolder* holder);
void requestStartPeachOnKoopaHitReactionBreakGateFinal(const al::IUseSceneObjHolder* holder);

}  // namespace rs
