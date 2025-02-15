#include "System/PlayerHitPointData.h"

#include "Library/Math/MathUtil.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/Writer/ByamlWriter.h"

PlayerHitPointData::PlayerHitPointData() {}

void PlayerHitPointData::setKidsModeFlag(bool kidsMode) {
    mIsKidsMode = kidsMode;
}

void PlayerHitPointData::init() {
    mIsHaveMaxUpItem = false;
    mIsForceNormalMode = false;
    if (mIsKidsMode)
        mCurrentHealth = 6;
    else
        mCurrentHealth = 3;
}

void PlayerHitPointData::recoverMax() {
    mCurrentHealth = getMaxCurrent();
}

s32 PlayerHitPointData::PlayerHitPointData::getCurrent() const {
    return mCurrentHealth;
}

s32 PlayerHitPointData::getMaxCurrent() const {
    if (mIsForceNormalMode)
        return 3;

    if (mIsHaveMaxUpItem)
        return getMaxWithItem();
    return getMaxWithoutItem();
}

s32 PlayerHitPointData::getMaxWithItem() const {
    if (mIsKidsMode && !mIsForceNormalMode)
        return 9;
    return 6;
}

s32 PlayerHitPointData::getMaxWithoutItem() const {
    if (mIsKidsMode && !mIsForceNormalMode)
        return 6;
    return 3;
}

bool PlayerHitPointData::isMaxCurrent() const {
    return getMaxCurrent() == mCurrentHealth;
}

bool PlayerHitPointData::isMaxWithItem() const {
    return getMaxWithItem() == mCurrentHealth;
}

void PlayerHitPointData::getMaxUpItem() {
    mIsHaveMaxUpItem = true;
    recoverMax();
}

void PlayerHitPointData::recover() {
    mCurrentHealth = sead::Mathi::min(mCurrentHealth + 1, getMaxCurrent());
}

void PlayerHitPointData::recoverForDebug() {
    if (mCurrentHealth >= getMaxCurrent() && !mIsHaveMaxUpItem)
        mIsHaveMaxUpItem = true;

    recover();
}

void PlayerHitPointData::damage() {
    s32 health = mCurrentHealth - 1;
    mCurrentHealth = sead::Mathi::max(health, 0);

    if (!mIsForceNormalMode && mCurrentHealth <= getMaxWithoutItem())
        mIsHaveMaxUpItem = false;
}

void PlayerHitPointData::kill() {
    mCurrentHealth = 0;
    if (!mIsForceNormalMode)
        mIsHaveMaxUpItem = false;
}

void PlayerHitPointData::forceNormalMode() {
    mIsForceNormalMode = true;
    mCurrentHealth = 3;
}

void PlayerHitPointData::endForceNormalMode() {
    mIsForceNormalMode = false;
    recoverMax();
}

bool PlayerHitPointData::isForceNormalMode() const {
    return mIsForceNormalMode;
}

void PlayerHitPointData::write(al::ByamlWriter* writer) {
    writer->pushHash("PlayerHitPointData");
    writer->addBool("IsHaveMaxUpItem", mIsHaveMaxUpItem);
    writer->pop();
}

void PlayerHitPointData::read(const al::ByamlIter& reader) {
    init();

    al::ByamlIter hitPointData{};
    reader.tryGetIterByKey(&hitPointData, "PlayerHitPointData");
    hitPointData.tryGetBoolByKey(&mIsHaveMaxUpItem, "IsHaveMaxUpItem");

    recoverMax();
}
