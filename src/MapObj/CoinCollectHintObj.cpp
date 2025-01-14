#include "MapObj/CoinCollectHintObj.h"

#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Scene/SceneUtil.h"

#include "Item/CoinCollectHolder.h"
#include "Scene/SceneObjFactory.h"

CoinCollectHintObj::CoinCollectHintObj(const char* name) : al::LiveActor(name) {}

void CoinCollectHintObj::init(const al::ActorInitInfo& initInfo) {
    al::initActorSceneInfo(this, initInfo);
    al::getStringArg(&mStageName, initInfo, "CoinCollectStageName");
    al::getTrans(&mTrans, initInfo);

    rs::createCoinCollectHolder(this);
    CoinCollectHolder* holder =
        (CoinCollectHolder*)al::getSceneObj(this, SceneObjID_CoinCollectHolder);
    holder->registerHintObj(this);

    makeActorDead();
}
