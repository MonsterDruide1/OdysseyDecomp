#include "Amiibo/HelpAmiiboLifeMaxUpItem.h"

#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nfp/NfpFunction.h"
#include "Library/Se/SeFunction.h"

#include "Amiibo/HelpAmiiboDirector.h"
#include "Amiibo/HelpAmiiboFunction.h"
#include "Item/LifeMaxUpItem.h"
#include "Item/LifeMaxUpItem2D.h"
#include "Util/PlayerUtil.h"

HelpAmiiboLifeMaxUpItem::HelpAmiiboLifeMaxUpItem(HelpAmiiboDirector* director, al::LiveActor* actor)
    : HelpAmiiboExecutor(director, actor, "ライフアップアイテムお助け") {}

void HelpAmiiboLifeMaxUpItem::initAfterPlacement(const al::ActorInitInfo& initInfo) {
    HelpAmiiboExecutor::initAfterPlacement(initInfo);
    mItem = new LifeMaxUpItem("最大ライフアップアイテム[amiibo]");
    al::initCreateActorNoPlacementInfo(mItem, initInfo);
    mItem->makeActorDead();

    mItem2D = new LifeMaxUpItem2D("最大ライフアップアイテム2D[amiibo]");
    al::initCreateActorNoPlacementInfo(mItem2D, initInfo);
    mItem2D->makeActorDead();
}

bool HelpAmiiboLifeMaxUpItem::isTriggerTouch(const al::NfpInfo& nfpInfo) const {
    return al::isCharacterIdBasePeach(nfpInfo);
}

bool HelpAmiiboLifeMaxUpItem::isEnableUse() {
    return true;
}

bool HelpAmiiboLifeMaxUpItem::execute() {
    bool isAlive = true;
    if (!al::isDead(mItem) || !al::isDead(mItem2D))
        isAlive = false;

    return isAlive;
}

void HelpAmiiboLifeMaxUpItem::activate() {
    al::LiveActor* actor = getActor();
    const sead::Vector3f& playerPos = rs::getPlayerPos(actor);
    HelpAmiiboExecutor::activate();

    if (rs::isPlayer2D(actor)) {
        sead::Vector3f appearOffset = sead::Vector3f::zero;
        HelpAmiiboFunction::calcLifeUpItemAppearOffset(&appearOffset, actor);
        sead::Vector3f frontDir = sead::Vector3f::zero;
        rs::calcPlayerFrontDir(&frontDir, actor);
        mItem2D->appearByAmiibo(playerPos + appearOffset, frontDir);
        al::startSe(getDirector(), "AmiiboPeach");
        return;
    }

    sead::Vector3f appearOffset = sead::Vector3f::zero;
    HelpAmiiboFunction::calcLifeUpItemAppearOffset(&appearOffset, actor);
    if (!rs::isPlayerHack(actor)) {
        mItem->appearAmiiboTouch(playerPos + appearOffset);
    } else {
        sead::Vector3f basePos = sead::Vector3f::zero;
        rs::calcPlayerAmiiboPeachAppearBasePos(&basePos, actor);
        mItem->appearAmiiboTouch(basePos);
    }
    al::startSe(getDirector(), "AmiiboPeach");
}

HelpAmiiboType HelpAmiiboLifeMaxUpItem::getType() const {
    return HelpAmiiboType::Peach;
}
