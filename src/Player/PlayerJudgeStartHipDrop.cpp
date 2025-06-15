#include "Player/PlayerJudgeStartHipDrop.h"

#include "Player/IPlayerModelChanger.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerInput.h"
#include "Util/PlayerCollisionUtil.h"

PlayerJudgeStartHipDrop::PlayerJudgeStartHipDrop(const PlayerConst* pConst,
                                                 const PlayerInput* input,
                                                 const IUsePlayerHeightCheck* heightCheck,
                                                 const IPlayerModelChanger* modelChanger)
    : mConst(pConst), mInput(input), mHeightCheck(heightCheck), mModelChanger(modelChanger) {}

bool PlayerJudgeStartHipDrop::judge() const {
    if (mModelChanger->is2DModel() || !mInput->isTriggerHipDrop())
        return false;

    return !(rs::getGroundHeight(mHeightCheck) < mConst->getHipDropHeight());
}
