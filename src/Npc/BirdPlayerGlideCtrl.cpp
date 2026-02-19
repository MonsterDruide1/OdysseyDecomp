#include "Npc/BirdPlayerGlideCtrl.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Resource/ResourceFunction.h"
#include "Library/Scene/SceneObjUtil.h"
#include "Library/Yaml/ByamlUtil.h"

#include "Npc/Bird.h"
#include "System/GameDataFunction.h"
#include "Util/DemoUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/ScenePlayerCapFunction.h"

namespace {
NERVE_IMPL(BirdPlayerGlideCtrl, Invalid);
NERVE_IMPL(BirdPlayerGlideCtrl, ValidOnNose);
NERVE_IMPL(BirdPlayerGlideCtrl, WaitFlyAway);
NERVE_IMPL_(BirdPlayerGlideCtrl, ValidOnSitDownHeadNoCap, ValidOnSitDownHead);
NERVE_IMPL_(BirdPlayerGlideCtrl, ValidOnSitDownHeadWithCap, ValidOnSitDownHead);

NERVES_MAKE_STRUCT(BirdPlayerGlideCtrl, Invalid, ValidOnNose, WaitFlyAway, ValidOnSitDownHeadNoCap,
                   ValidOnSitDownHeadWithCap);
}  // namespace

static const sead::Vector3f gDefaultCapOffset = {0, -50, 0};

BirdPlayerGlideCtrl::BirdPlayerGlideCtrl(const char* name)
    : al::LiveActor(name), mCapOffset(gDefaultCapOffset) {}

static void setBirdScale(Bird* bird) {
    if (al::isEqualString(al::getModelName(bird), "BirdCloud") ||
        al::isEqualString(al::getModelName(bird), "BirdCity") ||
        al::isEqualString(al::getModelName(bird), "BirdSky"))
        al::setScaleAll(bird, 0.875f);
    else
        al::setScaleAll(bird, 0.7f);
}

void BirdPlayerGlideCtrl::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initActorPoseTRSV(this);
    al::initActorSRT(this, info);
    al::initActorClipping(this, info);
    al::invalidateClipping(this);
    al::initNerve(this, &NrvBirdPlayerGlideCtrl.Invalid, 0);
    al::initExecutorMapObjMovement(this, info);

    mCommonBird = new Bird(al::getLinksActorDisplayName(info, "Bird", 0));
    al::initLinksActor(mCommonBird, info, "Bird", 0);
    setBirdScale(mCommonBird);
    mCommonBird->initGlideOff(&mDestinationMtx, sead::Vector3f::zero, true);
    mCommonBird->makeActorDead();

    mUfoBird = Bird::createBirdGlideDownUfo("月の鳥", info);
    setBirdScale(mUfoBird);
    mUfoBird->initGlideOff(&mDestinationMtx, sead::Vector3f::zero, true);

    al::setSceneObj(this, this, sSceneObjId);

    al::Resource* resource = al::findOrCreateResourceSystemData("BirdPlayerGlideInfo", nullptr);
    {
        al::ByamlIter iter{al::getByml(resource, "CapOffsetInfo")};

        s32 len = iter.getSize();
        if (len > 0) {
            mCapOffsetInfo.allocBuffer(len, nullptr);

            for (s32 i = 0; i < iter.getSize(); ++i) {
                al::ByamlIter subiter;
                al::getByamlIterByIndex(&subiter, iter, i);
                sead::Vector3f value = {0, 0, 0};
                al::tryGetByamlV3f(&value, subiter, "Offset");
                const char* name = al::getByamlKeyString(subiter, "CapName");
                mCapOffsetInfo.insert(name, value);
            }
        }
    }
    {
        al::ByamlIter iter{al::getByml(resource, "CostumeOffsetScaleInfo")};

        s32 len = iter.getSize();
        if (len > 0) {
            mCostumeOffsetScaleInfo.allocBuffer(len, nullptr);

            for (s32 i = 0; i < iter.getSize(); ++i) {
                al::ByamlIter subiter;
                al::getByamlIterByIndex(&subiter, iter, i);
                sead::Vector3f value = {0, 0, 0};
                al::tryGetByamlV3f(&value, subiter, "OffsetScale");
                const char* name = al::getByamlKeyString(subiter, "CostumeName");
                mCostumeOffsetScaleInfo.insert(name, value);
            }
        }
    }

    makeActorAlive();
}

void BirdPlayerGlideCtrl::initAfterPlacement() {
    if (mCapOffsetInfo.isBufferReady()) {
        const char* name = GameDataFunction::getCurrentCapTypeName(this);
        if (auto* node = mCapOffsetInfo.find(name))
            mCapOffset.set(node->value());
    }
    if (mCostumeOffsetScaleInfo.isBufferReady()) {
        const char* name = GameDataFunction::getCurrentCostumeTypeName(this);
        if (auto* node = mCostumeOffsetScaleInfo.find(name))
            mCostumeOffsetScale.set(node->value());
    }
}

void BirdPlayerGlideCtrl::validateGlideOnNose() {
    mIsValidOnNose = true;
    if (al::isNerve(this, &NrvBirdPlayerGlideCtrl.Invalid)) {
        al::setNerve(this, &NrvBirdPlayerGlideCtrl.ValidOnNose);
        return;
    }
    if (!al::isNerve(this, &NrvBirdPlayerGlideCtrl.ValidOnNose) && mBird)
        al::setNerve(this, &NrvBirdPlayerGlideCtrl.WaitFlyAway);
}

void BirdPlayerGlideCtrl::invalidateGlideOnNose() {
    mIsValidOnNose = false;
    if (mBird)
        al::setNerve(this, &NrvBirdPlayerGlideCtrl.WaitFlyAway);
    else if (mIsValidOnSitDownHead)
        if (PlayerCapFunction::isEnableBirdLandPlayerCapOn(this))
            al::setNerve(this, &NrvBirdPlayerGlideCtrl.ValidOnSitDownHeadWithCap);
        else
            al::setNerve(this, &NrvBirdPlayerGlideCtrl.ValidOnSitDownHeadNoCap);
    else
        al::setNerve(this, &NrvBirdPlayerGlideCtrl.Invalid);
}

void BirdPlayerGlideCtrl::validateGlideOnSitDownHead() {
    mIsValidOnSitDownHead = true;
    if (mIsValidOnNose)
        return;
    if (al::isNerve(this, &NrvBirdPlayerGlideCtrl.ValidOnSitDownHeadWithCap) ||
        al::isNerve(this, &NrvBirdPlayerGlideCtrl.ValidOnSitDownHeadNoCap))
        return;
    if (mBird)
        al::setNerve(this, &NrvBirdPlayerGlideCtrl.WaitFlyAway);
    else if (PlayerCapFunction::isEnableBirdLandPlayerCapOn(this))
        al::setNerve(this, &NrvBirdPlayerGlideCtrl.ValidOnSitDownHeadWithCap);
    else
        al::setNerve(this, &NrvBirdPlayerGlideCtrl.ValidOnSitDownHeadNoCap);
}

void BirdPlayerGlideCtrl::invalidateGlideOnSitDownHead() {
    mIsValidOnSitDownHead = false;
    if (!al::isNerve(this, &NrvBirdPlayerGlideCtrl.ValidOnSitDownHeadWithCap) &&
        !al::isNerve(this, &NrvBirdPlayerGlideCtrl.ValidOnSitDownHeadNoCap))
        return;
    if (mBird)
        al::setNerve(this, &NrvBirdPlayerGlideCtrl.WaitFlyAway);
    else
        al::setNerve(this, &NrvBirdPlayerGlideCtrl.Invalid);
}

void BirdPlayerGlideCtrl::addDemoActorAndFlyAway() {
    if (!mBird)
        return;
    rs::addDemoActor(this, false);
    rs::addDemoActor(mBird, false);
    al::setNerve(this, &NrvBirdPlayerGlideCtrl.WaitFlyAway);
}

void BirdPlayerGlideCtrl::exeInvalid() {
    if (al::isFirstStep(this) && mBird && al::isAlive(mBird)) {
        mBird->tryStartFlyAway();
        mBird = nullptr;
    }
}

static bool tryCalcGlideOnNoseMtx(sead::Matrix34f* out, const al::LiveActor* actor) {
    sead::Matrix34f mtx = sead::Matrix34f::ident;
    if (!rs::tryCalcPlayerModelNoseJointMtx(&mtx, actor))
        return false;
    sead::Vector3f side = mtx.getBase(0);
    sead::Vector3f up = mtx.getBase(1);
    sead::Vector3f front = mtx.getBase(2);
    sead::Vector3f pos = mtx.getBase(3);
    sead::Vector3f offset = {13, -2.5, 0};
    al::makeMtxFrontUpPos(out, front, up, pos + offset.x * side + offset.y * up + offset.z * front);
    return true;
}

void BirdPlayerGlideCtrl::exeValidOnNose() {
    if (al::isFirstStep(this))
        mGlideDownTime = al::getRandom(180, 1200);
    if (al::isGreaterEqualStep(this, mGlideDownTime)) {
        if (al::isStep(this, mGlideDownTime)) {
            if (!mBird) {
                mBird = GameDataFunction::isGameClear(this) && al::getRandom() < 0x1p-9 ?
                            mUfoBird :
                            mCommonBird;
            }
            if (al::isDead(mBird) && tryCalcGlideOnNoseMtx(&mDestinationMtx, mBird))
                mBird->tryStartGlideDown();
        }
        if (al::isAlive(mBird))
            tryCalcGlideOnNoseMtx(&mDestinationMtx, mBird);
    }
}

static bool tryCalcGlideOnSitDownHeadMtx(sead::Matrix34f* out, const al::LiveActor* actor,
                                         const sead::Vector3f& offset) {
    sead::Vector3f pos = {0, 0, 0};
    sead::Vector3f front = {0, 0, 0};
    sead::Vector3f up = {0, 0, 0};
    sead::Vector3f side = {0, 0, 0};
    if (!rs::tryCalcPlayerModelHeadJointPos(&pos, actor) ||
        !rs::tryCalcPlayerModelHeadJointFront(&front, actor) ||
        !rs::tryCalcPlayerModelHeadJointUp(&up, actor) ||
        !rs::tryCalcPlayerModelHeadJointSide(&side, actor))
        return false;
    al::makeMtxFrontUpPos(out, front, up, pos + offset.x * side + offset.y * up + offset.z * front);
    return true;
}

static const sead::Vector3f gNoCapOffset = {0, -42.5, 0};

void BirdPlayerGlideCtrl::exeValidOnSitDownHead() {
    if (al::isFirstStep(this))
        mGlideDownTime = al::getRandom(180, 1200);
    if (al::isGreaterEqualStep(this, mGlideDownTime)) {
        sead::Vector3f cap_offset;
        if (al::isNerve(this, &NrvBirdPlayerGlideCtrl.ValidOnSitDownHeadWithCap))
            cap_offset = mCapOffset;
        else
            cap_offset = gNoCapOffset;
        sead::Vector3f costume_offset_scale = mCostumeOffsetScale;
        sead::Vector3f offset = {cap_offset.x * costume_offset_scale.x,
                                 cap_offset.y * costume_offset_scale.y,
                                 cap_offset.z * costume_offset_scale.z};
        if (tryCalcGlideOnSitDownHeadMtx(&mDestinationMtx, this, offset) &&
            al::isStep(this, mGlideDownTime)) {
            if (!mBird)
                mBird = mCommonBird;
            mBird->tryStartGlideDown();
        }
    }
    if (al::isNerve(this, &NrvBirdPlayerGlideCtrl.ValidOnSitDownHeadWithCap) !=
        PlayerCapFunction::isEnableBirdLandPlayerCapOn(this)) {
        al::setNerve(this, &NrvBirdPlayerGlideCtrl.WaitFlyAway);
    }
}

void BirdPlayerGlideCtrl::exeWaitFlyAway() {
    if (mBird && !al::isDead(mBird))
        mBird->tryStartFlyAway();
    else if (mIsValidOnNose)
        al::setNerve(this, &NrvBirdPlayerGlideCtrl.ValidOnNose);
    else if (mIsValidOnSitDownHead)
        if (PlayerCapFunction::isEnableBirdLandPlayerCapOn(this))
            al::setNerve(this, &NrvBirdPlayerGlideCtrl.ValidOnSitDownHeadWithCap);
        else
            al::setNerve(this, &NrvBirdPlayerGlideCtrl.ValidOnSitDownHeadNoCap);
    else
        al::setNerve(this, &NrvBirdPlayerGlideCtrl.Invalid);
}

namespace rs {

void validateGlideBirdOnPlayerNose(const al::LiveActor* player) {
    if (al::isExistSceneObj<BirdPlayerGlideCtrl>(player))
        al::getSceneObj<BirdPlayerGlideCtrl>(player)->validateGlideOnNose();
}

void invalidateGlideBirdOnPlayerNose(const al::LiveActor* player) {
    if (al::isExistSceneObj<BirdPlayerGlideCtrl>(player))
        al::getSceneObj<BirdPlayerGlideCtrl>(player)->invalidateGlideOnNose();
}

void validateGlideBirdOnSitDownPlayerHead(const al::LiveActor* player) {
    if (al::isExistSceneObj<BirdPlayerGlideCtrl>(player))
        al::getSceneObj<BirdPlayerGlideCtrl>(player)->validateGlideOnSitDownHead();
}

void invalidateGlideBirdOnSitDownPlayerHead(const al::LiveActor* player) {
    if (al::isExistSceneObj<BirdPlayerGlideCtrl>(player))
        al::getSceneObj<BirdPlayerGlideCtrl>(player)->invalidateGlideOnSitDownHead();
}

bool isPlayerSitDownChair(const Bird* bird) {
    return al::isExistSceneObj<BirdPlayerGlideCtrl>(bird) &&
           al::getSceneObj<BirdPlayerGlideCtrl>(bird)->isValidOnSitDownHead();
}

}  // namespace rs
