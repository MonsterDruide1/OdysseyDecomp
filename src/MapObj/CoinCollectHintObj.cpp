#include "MapObj/CoinCollectHintObj.h"

#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Scene/SceneObjUtil.h"

#include "Item/CoinCollectHolder.h"
#include "Scene/SceneObjFactory.h"

CoinCollectHintObj::CoinCollectHintObj(const char* name) : al::LiveActor(name) {}

void CoinCollectHintObj::init(const al::ActorInitInfo& initInfo) {
    al::initActorSceneInfo(this, initInfo);
    al::getStringArg(&mStageName, initInfo, "CoinCollectStageName");
    al::getTrans(&mTrans, initInfo);

    rs::createCoinCollectHolder(this);
    CoinCollectHolder* holder =
        al::getSceneObj<CoinCollectHolder>(this, SceneObjID_CoinCollectHolder);
    holder->registerHintObj(this);

    makeActorDead();
}
