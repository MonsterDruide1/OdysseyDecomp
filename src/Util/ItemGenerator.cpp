#include "Util/ItemGenerator.h"

#include <math/seadVectorCalcCommon.h>

#include "Library/Item/ItemUtil.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"

#include "Enemy/KuriboMini.h"
#include "Util/ItemUtil.h"

ItemGenerator::ItemGenerator() {}

ItemGenerator::ItemGenerator(al::LiveActor* creator, const al::ActorInitInfo& info)
    : mCreator(creator) {
    initLinkShine(creator, info, false);
}

void ItemGenerator::initLinkShine(al::LiveActor* creator, const al::ActorInitInfo& info,
                                  bool isAppearAbove) {
    mCreator = creator;
    mItemType = rs::getItemType(info);

    if (isShine()) {
        mLinkShine = rs::tryInitLinkShine(info, "ShineActor", 0);
    } else if (rs::isItemTypeKuriboMini(&mKuriboMiniCount, mItemType)) {
        mKuriboMiniArray = new KuriboMini*[mKuriboMiniCount];
        for (s32 i = 0; i < mKuriboMiniCount; i++) {
            mKuriboMiniArray[i] = new KuriboMini("マメクリボー[アイテム]");
            al::initCreateActorNoPlacementInfo(mKuriboMiniArray[i], info);
            mKuriboMiniArray[i]->makeActorDead();
        }
    } else {
        rs::tryInitItemByPlacementInfo(mCreator, info, isAppearAbove);
    }
}

void ItemGenerator::initNoLinkShine(al::LiveActor* creator, const al::ActorInitInfo& info,
                                    bool isAppearAbove) {
    mCreator = creator;
    mItemType = rs::getItemType(info);

    if (isShine()) {
        mLinkShine = rs::initShineByPlacementInfo(info);
    } else if (rs::isItemTypeKuriboMini(&mKuriboMiniCount, mItemType)) {
        mKuriboMiniArray = new KuriboMini*[mKuriboMiniCount];
        for (s32 i = 0; i < mKuriboMiniCount; i++) {
            mKuriboMiniArray[i] = new KuriboMini("マメクリボー[アイテム]");
            al::initCreateActorNoPlacementInfo(mKuriboMiniArray[i], info);
            mKuriboMiniArray[i]->makeActorDead();
        }
    } else {
        rs::tryInitItemByPlacementInfo(mCreator, info, isAppearAbove);
    }
}

void ItemGenerator::initHintPhotoShine(al::LiveActor* creator, const al::ActorInitInfo& info) {
    mCreator = creator;
    mItemType = rs::ItemType::Shine;
    mLinkShine = rs::tryInitLinkShineHintPhoto(info, "ShineActor", 0);
}

void ItemGenerator::createShineEffectInsideObject(const al::ActorInitInfo& info) {
    rs::createShineEffectInsideObject(mLinkShine, mCreator, info);
}

bool ItemGenerator::tryUpdateHintTransIfExistShine() {
    const sead::Vector3f& trans = al::getTrans(mCreator);
    return tryUpdateHintTransIfExistShine(trans);
}

bool ItemGenerator::tryUpdateHintTransIfExistShine(const sead::Vector3f& trans) {
    if (!mLinkShine || mGeneratedItemCount > 0)
        return false;

    rs::updateHintTrans(mLinkShine, trans);
    return true;
}

void generateKuribos(KuriboMini** kuriboMiniArray, s32 kuriboMiniCount, al::LiveActor* creator,
                     const sead::Vector3f& front) {
    sead::Vector3f up = sead::Vector3f(0.0f, 1.0f, 0.0f);
    al::verticalizeVec(&up, front, up);

    if (!al::tryNormalizeOrZero(&up))
        al::calcDirVerticalAny(&up, front);

    for (s32 i = 0; i < kuriboMiniCount; i++) {
        sead::Vector3f frontDir = front;
        al::rotateVectorDegree(&frontDir, frontDir, up,
                               al::normalize(i, 0, kuriboMiniCount) * 360.0f);
        al::normalize(&frontDir);

        sead::Quatf quat = sead::Quatf::unit;
        al::makeQuatFrontUp(&quat, frontDir, up);
        al::updatePoseQuat(kuriboMiniArray[i], quat);

        sead::Vector3f pos = sead::Vector3f(0.0f, 0.0f, 150.0f);
        pos.rotate(quat);

        al::setTrans(kuriboMiniArray[i], al::getTrans(creator) - pos);
        kuriboMiniArray[i]->appearPopBack();
    }
}

bool ItemGenerator::isEnableGenerateByCount(s32 count) const {
    return mGeneratedItemCount < count;
}

void ItemGenerator::generate(const sead::Vector3f& pos, const sead::Quatf& quat) {
    if (isNone())
        return;

    KuriboMini** kuriboMiniArray = mKuriboMiniArray;
    if (mLinkShine)
        rs::appearPopupShine(mLinkShine, pos);
    else if (kuriboMiniArray) {
        al::LiveActor* creator = mCreator;
        s32 kuriboMiniCount = mKuriboMiniCount;
        sead::Vector3f vec = sead::Vector3f(0.0f, 0.0f, 0.0f);
        al::calcQuatFront(&vec, quat);
        generateKuribos(kuriboMiniArray, kuriboMiniCount, creator, vec);
    } else
        al::appearItem(mCreator, pos, quat, nullptr);

    mGeneratedItemCount++;
}

void ItemGenerator::generate(const sead::Vector3f& pos, const sead::Vector3f& vec) {
    if (isNone())
        return;

    KuriboMini** kuriboMiniArray = mKuriboMiniArray;

    if (mLinkShine)
        rs::appearPopupShine(mLinkShine, pos);
    else if (kuriboMiniArray)
        generateKuribos(mKuriboMiniArray, mKuriboMiniCount, mCreator, vec);
    else
        al::appearItem(mCreator, pos, vec, nullptr);

    mGeneratedItemCount++;
}

bool ItemGenerator::tryGenerate(const sead::Vector3f& pos, const sead::Quatf& quat, s32 count) {
    if (isNone() || !isEnableGenerateByCount(count))
        return false;

    generate(pos, quat);
    return true;
}

bool ItemGenerator::tryGenerate(const sead::Vector3f& pos, const sead::Vector3f& vec, s32 count) {
    if (isNone() || !isEnableGenerateByCount(count))
        return false;

    generate(pos, vec);
    return true;
}

bool ItemGenerator::isNone() const {
    return mItemType == rs::ItemType::None;
}

bool ItemGenerator::isShine() const {
    return mItemType == rs::ItemType::Shine;
}

bool ItemGenerator::isLifeUp() const {
    return mItemType == rs::ItemType::LifeUpItem || mItemType == rs::ItemType::LifeMaxUpItem;
}

bool ItemGenerator::isLifeMaxUp() const {
    return mItemType == rs::ItemType::LifeMaxUpItem;
}

bool ItemGenerator::isCoin() const {
    return mItemType == rs::ItemType::Coin;
}

bool ItemGenerator::isCoinBlow() const {
    return mItemType == rs::ItemType::CoinBlow;
}

bool ItemGenerator::isCoinStackBound() const {
    return mItemType == rs::ItemType::CoinStackBound;
}

bool ItemGenerator::isKuriboMini3() const {
    return mItemType == rs::ItemType::KuriboMini3;
}

bool ItemGenerator::isKuriboMini8() const {
    return mItemType == rs::ItemType::KuriboMini8;
}
