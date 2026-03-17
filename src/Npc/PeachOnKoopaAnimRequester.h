#pragma once

#include <basis/seadTypes.h>

#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
class IUseSceneObjHolder;
}

class PeachOnKoopaAnimRequester : public al::ISceneObj {
public:
    PeachOnKoopaAnimRequester();

    const char* getSceneObjName() const override;

    void resetRequest();

    static constexpr s32 sSceneObjId = SceneObjID_PeachOnKoopaAnimRequester;

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
