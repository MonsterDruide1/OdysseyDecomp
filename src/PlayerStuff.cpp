#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Player/PlayerActorBase.h"
#include "PlayerUtil.h"
#include "playerUtil.h"


void rs::initPlayerActorInfo(PlayerActorBase* player, PlayerInitInfo const& info) {
    player->mPortNo = info.mPortNo;
    player->mViewMtx = info.mViewMtx;

    sead::Matrix34f mtx;
    mtx.makeQT(info._34, info._20);
    al::updatePoseMtx(player, &mtx);

}
