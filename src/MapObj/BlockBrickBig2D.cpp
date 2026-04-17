#include "MapObj/BlockBrickBig2D.h"

#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "Util/ActorDimensionUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(BlockBrickBig2D, Wait)

NERVES_MAKE_STRUCT(BlockBrickBig2D, Wait)
}  // namespace

BlockBrickBig2D::BlockBrickBig2D(const char* name) : al::LiveActor(name) {}

void BlockBrickBig2D::init(const al::ActorInitInfo& info) {
    using BlockBrickBig2DFunctor = al::FunctorV0M<BlockBrickBig2D*, void (BlockBrickBig2D::*)()>;

    al::initActor(this, info);
    al::initNerve(this, &NrvBlockBrickBig2D.Wait, 0);
    mMtxConnector = al::tryCreateMtxConnector(this, info);
    mDimensionKeeper = rs::createDimensionKeeper(this);
    rs::updateDimensionKeeper(mDimensionKeeper);
    rs::snap2DParallelizeFront(this, this, 500.0f);
    al::listenStageSwitchOn(this, "SwitchBreak",
                            BlockBrickBig2DFunctor(this, &BlockBrickBig2D::startBreak));
    al::trySyncStageSwitchAppear(this);
}

void BlockBrickBig2D::startBreak() {
    if (al::isAlive(this))
        kill();
}

void BlockBrickBig2D::initAfterPlacement() {
    if (mMtxConnector)
        al::attachMtxConnectorToCollision(mMtxConnector, this, false);
}

void BlockBrickBig2D::kill() {
    al::getSubActor(this, "壊れモデル")->appear();
    al::LiveActor::kill();
}

bool BlockBrickBig2D::receiveMsg(const al::SensorMsg* msg, al::HitSensor* other,
                                 al::HitSensor* self) {
    if (!rs::isMsgBlockReaction2D(msg))
        return false;

    kill();
    return true;
}

void BlockBrickBig2D::exeWait() {
    if (mMtxConnector)
        al::connectPoseQT(this, mMtxConnector);
}
