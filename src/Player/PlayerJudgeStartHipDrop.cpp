#include "Player/PlayerJudgeStartHipDrop.h"

#include "Player/IPlayerModelChanger.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerInput.h"
#include "Util/PlayerCollisionUtil.h"

PlayerJudgeStartHipDrop::PlayerJudgeStartHipDrop(const PlayerConst* playerConst,
                                                 const PlayerInput* playerInput,
                                                 const IUsePlayerHeightCheck* playerHeightCheck,
                                                 const IPlayerModelChanger* playerModelChanger)
    : mConst(playerConst), mInput(playerInput), mHeightCheck(playerHeightCheck),
      mModelChanger(playerModelChanger) {}

bool PlayerJudgeStartHipDrop::judge() const {
    if (mModelChanger->is2DModel() || !mInput->isTriggerHipDrop())
        return false;

    return !(rs::getGroundHeight(mHeightCheck) < mConst->getHipDropHeight());
}
