#include "Item/CoinChameleon.h"

#include "Library/Collision/PartsConnector.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "Item/Coin.h"
#include "Item/CoinRotateCalculator.h"
#include "Item/CoinStateAppearRotate.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(CoinChameleon, Wait);
NERVE_IMPL(CoinChameleon, Visible);
NERVE_IMPL(CoinChameleon, Appear);

NERVES_MAKE_STRUCT(CoinChameleon, Wait, Appear, Visible);
}  // namespace

CoinChameleon::CoinChameleon(const char* name) : al::LiveActor(name) {}

void CoinChameleon::init(const al::ActorInitInfo& initInfo) {
    al::initActor(this, initInfo);
    al::initNerve(this, &NrvCoinChameleon.Wait, 1);

    Coin* coin = new Coin("コイン", false);
    al::initCreateActorWithPlacementInfo(coin, initInfo);
    mCoin = coin;
    mCoin->makeActorDead();

    mQuat.set(al::getQuat(this));
    mRotateCalculator = new CoinRotateCalculator(this);
    al::setModelMaterialParameterF32(this, "CoinW6Mat00", "const_single0", 0.0f);
    al::hideModel(this);
    al::setMaterialProgrammable(this);

    mMtxConnector = al::tryCreateMtxConnector(this, initInfo);
    if (mMtxConnector != nullptr)
        mCoin->setMtxConnector(mMtxConnector);

    al::tryAddDisplayOffset(this, initInfo);
    al::tryGetDisplayOffset(&mDisplayOffset, initInfo);

    mStateAppearRotate = new CoinStateAppearRotate(this, mMtxConnector, mDisplayOffset, "出現");
    al::initNerveState(this, mStateAppearRotate, &NrvCoinChameleon.Appear, "出現");

    if (al::isPlaced(initInfo)) {
        f32 shadowLength = 1500.0f;
        al::tryGetArg(&shadowLength, initInfo, "ShadowLength");
        mCoin->setShadowDropLength(shadowLength);
    }
    makeActorAlive();
}

void CoinChameleon::initAfterPlacement() {
    if (mMtxConnector != nullptr)
        al::attachMtxConnectorToCollision(mMtxConnector, this, false);
}

void CoinChameleon::endClipped() {
    mRotateCalculator->reset();
    al::LiveActor::endClipped();
}

bool CoinChameleon::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                               al::HitSensor* self) {
    if (rs::isMsgPlayerDisregardTargetMarker(message))
        return true;
    if (al::isNerve(this, &NrvCoinChameleon.Wait) || al::isNerve(this, &NrvCoinChameleon.Visible)) {
        if (rs::isMsgItemGetAll(message) && al::isSensorName(self, "Body")) {
            al::invalidateClipping(this);
            al::setNerve(this, &NrvCoinChameleon.Appear);
            return true;
        }
        if (rs::isVisibleChameleon(message) && al::isSensorName(self, "Hipdrop")) {
            al::invalidateClipping(this);
            al::setNerve(this, &NrvCoinChameleon.Visible);
            return true;
        }
    }
    return false;
}

void CoinChameleon::rotate() {
    if (mMtxConnector == nullptr)
        al::setQuat(this, mQuat);

    bool checkWater = false;
    if (al::isNerve(this, &NrvCoinChameleon.Appear) || mMtxConnector != nullptr)
        checkWater = true;

    mRotateCalculator->update(sead::Vector3f::zero, checkWater);
    al::rotateQuatYDirDegree(this, mRotateCalculator->getRotate());
}

void CoinChameleon::exeWait() {
    if (al::isFirstStep(this)) {
        al::validateClipping(this);
        mWaitTime = 0;
    }
    if (mMtxConnector != nullptr) {
        al::connectPoseQT(this, mMtxConnector);
        *al::getTransPtr(this) += mDisplayOffset;
    }

    if (!rs::isPlayerEnableToSeeOddSpace(this)) {
        mWaitTime = sead::Mathi::clampMin(mWaitTime - 1, 0);
        al::setModelMaterialParameterF32(this, "CoinW6Mat00", "const_single0", mWaitTime / 15.0f);
        if (mWaitTime == 0)
            al::hideModelIfShow(this);
    } else {
        al::showModelIfHide(this);
        mWaitTime = sead::Mathi::clampMax(15, mWaitTime + 1);
        al::setModelMaterialParameterF32(this, "CoinW6Mat00", "const_single0", mWaitTime / 15.0f);
    }

    rotate();
}

void CoinChameleon::exeVisible() {
    if (al::isFirstStep(this)) {
        mWaitTime = 0;
        al::showModelIfHide(this);
    }

    if (mMtxConnector != nullptr) {
        al::connectPoseQT(this, mMtxConnector);
        *al::getTransPtr(this) += mDisplayOffset;
    }

    al::setModelMaterialParameterF32(this, "CoinW6Mat00", "const_single0",
                                     1.0f - al::getNerveStep(this) / 120.0f);

    if (al::isGreaterEqualStep(this, 120)) {
        al::hideModelIfShow(this);
        al::setNerve(this, &NrvCoinChameleon.Wait);
        return;
    }

    rotate();
}

void CoinChameleon::exeAppear() {
    if (al::updateNerveState(this)) {
        mCoin->appearCoinChameleon(al::getTrans(this), al::getQuat(this), mDisplayOffset);
        kill();
        return;
    }

    if (rs::isPlayerEnableToSeeOddSpace(this)) {
        al::setModelMaterialParameterF32(this, "CoinW6Mat00", "const_single0", 1.0f);
        return;
    }

    al::setModelMaterialParameterF32(this, "CoinW6Mat00", "const_single0",
                                     al::getNerveStep(this) / 20.0f);
}
