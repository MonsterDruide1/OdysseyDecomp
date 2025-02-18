#include "Amiibo/AmiiboNpcDirector.h"

#include <time/seadDateTime.h>

#include "Library/Audio/System/AudioKeeper.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Message/MessageTagDataHolder.h"
#include "Library/Nfp/NfpFunction.h"
#include "Library/Nfp/NfpTypes.h"
#include "Library/Se/SeFunction.h"

#include "Amiibo/SearchAmiiboDataTable.h"
#include "Layout/AmiiboNpcLayout.h"
#include "Layout/ShopLayoutInfo.h"
#include "System/GameDataFunction.h"
#include "System/ProjectNfpDirector.h"
#include "Util/AmiiboUtil.h"
#include "Util/ClothUtil.h"
#include "Util/TimeUtil.h"

AmiiboNpcDirector::AmiiboNpcDirector() : mNfpInfo(new al::NfpInfo()) {
    for (s32 i = 0; i < 3; i++)
        mAmiiboNameCstr[i] = nullptr;
}

const char* AmiiboNpcDirector::getSceneObjName() const {
    return "AmiiboNpc用データホルダ";
}

void AmiiboNpcDirector::init(ProjectNfpDirector* nfpDirector, al::AudioDirector* audioDirector) {
    mNfpDirector = nfpDirector;
    mAudioKeeper =
        alAudioKeeperFunction::createAudioKeeper(audioDirector, "AmiiboNpcDirector", nullptr);

    mTagDataHolder = new al::MessageTagDataHolder(6);
    al::registerMessageTagDataAmiiboName(mTagDataHolder, "First", &mAmiiboNameCstr[0]);
    al::registerMessageTagDataAmiiboName(mTagDataHolder, "Second", &mAmiiboNameCstr[1]);
    al::registerMessageTagDataAmiiboName(mTagDataHolder, "Third", &mAmiiboNameCstr[2]);
    al::registerMessageTagDataAmiiboName(mTagDataHolder, "Touch", &mTouchAmiiboNameCstr);
    al::registerMessageTagDataString(mTagDataHolder, "ClothName", &mClothName);
    al::registerMessageTagDataString(mTagDataHolder, "CapName", &mCapName);
}

void AmiiboNpcDirector::initAfterPlacementSceneObj(const al::ActorInitInfo& initInfo) {
    const al::LayoutInitInfo& layoutInitInfo = al::getLayoutInitInfo(initInfo);
    mNpcLayout = new AmiiboNpcLayout(layoutInitInfo);

    mSearchDataTable = rs::getSearchAmiiboData(mNpcLayout);
    updateSearchAmiiboName();

    const char* currentCostumeTypeName = GameDataFunction::getCurrentCostumeTypeName(mNpcLayout);
    const sead::PtrArray<ShopItem::ItemInfo>& clothList = rs::getClothList(mNpcLayout);

    for (s32 i = 0; i < clothList.size(); i++) {
        if (al::isEqualString(clothList.at(i)->mName, currentCostumeTypeName)) {
            mClothName = rs::getDisplayName(mNpcLayout, *clothList.at(i));
            break;
        }
    }

    const char* currentCapTypeName = GameDataFunction::getCurrentCapTypeName(mNpcLayout);
    const sead::PtrArray<ShopItem::ItemInfo>& capList = rs::getCapList(mNpcLayout);

    for (s32 i = 0; i < capList.size(); i++) {
        if (al::isEqualString(capList.at(i)->mName, currentCapTypeName)) {
            mCapName = rs::getDisplayName(mNpcLayout, *capList.at(i));
            break;
        }
    }
}

void AmiiboNpcDirector::updateSearchAmiiboName() {
    s32 dataNumMax = mSearchDataTable->getDataNumMax();
    s32 assignedIndex = 0;

    for (s32 i = 0; i < dataNumMax; i++) {
        if (mSearchDataTable->isInvalidId(i))
            continue;

        bool isAssigned = false;
        mAmiiboName[i] =
            rs::getAmiiboMstxtLabel(&isAssigned, mNpcLayout, mSearchDataTable->getId(i),
                                    mSearchDataTable->getNumberingId(i), assignedIndex);
        mAmiiboNameCstr[i] = mAmiiboName[i].cstr();
        if (isAssigned)
            assignedIndex++;
    }
}

bool AmiiboNpcDirector::requestAppearAmiiboLayout() {
    if (isEndAmiiboLayout()) {
        mNpcLayout->appear();
        return true;
    }
    return false;
}

void AmiiboNpcDirector::requestDecideAmiiboLayout() {
    mNpcLayout->decide();
}

void AmiiboNpcDirector::requestEndAmiiboLayout() {
    mNpcLayout->end();
}

bool AmiiboNpcDirector::isEndAmiiboLayout() {
    return al::isDead(mNpcLayout);
}

void AmiiboNpcDirector::registerSearchAmiibo(s32 id, s32 numberingId, u64 searchStartTime) {
    for (s32 i = 0; i < mSearchDataTable->getDataNumMax(); i++) {
        if (mSearchDataTable->isInvalidId(i)) {
            mSearchDataTable->setId(id, numberingId, i);
            mSearchDataTable->setSearchStartTime(searchStartTime, i);
            updateSearchAmiiboName();
            return;
        }
    }
}

void AmiiboNpcDirector::deleteSearchEndAmiibo() {
    s32 assignedIndex = 0;
    for (s32 i = 0; i < mSearchDataTable->getDataNumMax(); i++) {
        if (mSearchDataTable->isInvalidId(i)) {
            if (i != assignedIndex) {
                mSearchDataTable->copy(assignedIndex, i);
                mSearchDataTable->initByIndex(i);
            }
            break;
        }

        if ((s64)mTime - (s64)mSearchDataTable->getSearchStartTime(i) > 300) {
            mSearchDataTable->initByIndex(i);
            continue;
        }

        if (i != assignedIndex) {
            mSearchDataTable->copy(assignedIndex, i);
            mSearchDataTable->initByIndex(i);
            assignedIndex++;
        } else {
            assignedIndex++;
        }
    }
    updateSearchAmiiboName();
}

bool AmiiboNpcDirector::isSearchAmiibo(s32 id) {
    for (s32 i = 0; i < mSearchDataTable->getDataNumMax(); i++)
        if (mSearchDataTable->getId(i) == id)
            return true;
    return false;
}

u32 AmiiboNpcDirector::getSearchAmiiboNum() const {
    u32 amiiboNum = 0;

    for (s32 i = 0; i < mSearchDataTable->getDataNumMax(); i++) {
        if (mSearchDataTable->isInvalidId(i))
            continue;
        if (mTime < mSearchDataTable->getSearchStartTime(i) + 300)
            amiiboNum++;
    }
    return amiiboNum;
}

u32 AmiiboNpcDirector::getSearchEndAmiiboNum() const {
    u32 amiiboNum = 0;

    for (s32 i = 0; i < mSearchDataTable->getDataNumMax(); i++) {
        if (mSearchDataTable->isInvalidId(i))
            continue;
        if (mTime > mSearchDataTable->getSearchStartTime(i) + 300)
            amiiboNum++;
    }
    return amiiboNum;
}

u32 AmiiboNpcDirector::getSearchEndAmiiboNumRealTime() const {
    u64 now = sead::DateTime(0).setNow();
    u32 amiiboNum = 0;

    for (s32 i = 0; i < mSearchDataTable->getDataNumMax(); i++) {
        if (mSearchDataTable->isInvalidId(i))
            continue;
        if (now - mSearchDataTable->getSearchStartTime(i) > 300)
            amiiboNum++;
    }
    return amiiboNum;
}

bool AmiiboNpcDirector::isEnableSearchAmiibo() {
    for (s32 i = 0; i < mSearchDataTable->getDataNumMax(); i++)
        if (mSearchDataTable->isInvalidId(i))
            return true;
    return false;
}

void AmiiboNpcDirector::setTouchAmiiboName(s32 id, s32 numberingId) {
    mTouchAmiiboName = rs::getAmiiboMstxtLabel(nullptr, mNpcLayout, id, numberingId, 0);
    mTouchAmiiboNameCstr = mTouchAmiiboName.cstr();
}

void AmiiboNpcDirector::trySetAmiiboCostumeName(s32 id) {
    al::NfpInfo* nfpInfo = mNfpInfo;
    al::NfpCharacterId charId{};
    al::tryGetCharacterId(&charId, *nfpInfo);

    s32 numberingId = -1;
    al::tryGetNumberingId(&numberingId, *nfpInfo);

    ShopItem::ItemInfo* itemA = nullptr;
    ShopItem::ItemInfo* itemB = nullptr;
    rs::tryFindAmiiboCostumeItemInfo(&itemA, &itemB, charId, numberingId, mNpcLayout);

    if (itemA != nullptr)
        mClothName = rs::getDisplayName(mNpcLayout, *itemA);
    if (itemB != nullptr)
        mCapName = rs::getDisplayName(mNpcLayout, *itemB);
}

void AmiiboNpcDirector::checkTimeReverseAndRestore() {
    s32 dataNumMax = mSearchDataTable->getDataNumMax();
    for (s32 i = 0; i < dataNumMax; i++) {
        if (mSearchDataTable->isInvalidId(i))
            continue;
        u64 startTime = mSearchDataTable->getSearchStartTime(i);
        if (rs::checkTimeReverseAndRestore(&startTime, mTime))
            mSearchDataTable->setSearchStartTime(startTime, i);
    }
}

al::NfpInfo* AmiiboNpcDirector::tryGetTriggerTouchNfpInfo() {
    al::NfpInfo* nfpInfo = mNfpDirector->tryGetTriggerTouchNfpInfo();
    if (nfpInfo == nullptr)
        return nullptr;

    *mNfpInfo = *nfpInfo;
    al::startSe(this, "TouchAmiibo");
    return mNfpInfo;
}

al::AudioKeeper* AmiiboNpcDirector::getAudioKeeper() const {
    return mAudioKeeper;
}
