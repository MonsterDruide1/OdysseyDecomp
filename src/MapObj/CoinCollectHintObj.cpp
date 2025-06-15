#include "MapObj/CoinCollectHintObj.h"

#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Scene/SceneObjUtil.h"

#include "Item/CoinCollectHolder.h"

CoinCollectHintObj::CoinCollectHintObj(const char* name) : al::LiveActor(name) {}

void CoinCollectHintObj::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::getStringArg(&mStageName, info, "CoinCollectStageName");
    al::getTrans(&mTrans, info);

    rs::createCoinCollectHolder(this);
    CoinCollectHolder* holder = al::getSceneObj<CoinCollectHolder>(this);
    holder->registerHintObj(this);

    makeActorDead();
}
