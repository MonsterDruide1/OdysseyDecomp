#include "Layout/ShopUtil.h"

#include "Library/Base/StringUtil.h"
#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Play/Layout/SimpleLayoutAppearWaitEnd.h"
#include "Library/Play/Layout/TalkMessageVoicePlayer.h"

void ShopUtil::makeExplainLabel(al::StringTmp<256>* result, const char* message,
                                const char* storeName, bool isWomanExplain) {
    const char* person = isWomanExplain ? "Woman" : "";
    result->format("%s%s%s", message, storeName, person);
}

void ShopUtil::startNpcMessage(al::SimpleLayoutAppearWaitEnd* layout, const char16* msgString,
                               al::LiveActor* actor, const char* actorName, bool* isSkipTextAnim,
                               al::TalkMessageVoicePlayer* voicePlayer) {
    *isSkipTextAnim = false;
    al::showPane(layout, "TxtMessage");

    al::startTextPaneAnimWithAudioUser(layout, msgString, nullptr, nullptr, actor);
    if (voicePlayer)
        voicePlayer->start(layout, actor, msgString, 0);
    al::startAction(actor, actorName);
}

void ShopUtil::startNpcMessage(al::SimpleLayoutAppearWaitEnd* layout, const char* messageName,
                               al::LiveActor* actor, const char* actorName, bool* isSkipTextAnim,
                               al::TalkMessageVoicePlayer* voicePlayer) {
    const char16* msgString = getSystemMessageStringForShop(layout, messageName);
    startNpcMessage(layout, msgString, actor, actorName, isSkipTextAnim, voicePlayer);
}

const char16* ShopUtil::getSystemMessageStringForShop(const al::IUseMessageSystem* messageSystem,
                                                      const char* messageName) {
    return al::getSystemMessageString(messageSystem, "ShopMessage", messageName);
}

const char* ShopUtil::getShopNameCityMan() {
    return "CityManShop";
}

const char* ShopUtil::getShopNameCityWoman() {
    return "CityWomanShop";
}

const char* ShopUtil::getShopNameSeaWoman() {
    return "SeaWoman";
}

const char* ShopUtil::getShopNameCapWoman() {
    return "CapWoman";
}

const char* ShopUtil::getShopNameCapMan() {
    return "CapMan";
}

bool ShopUtil::isShopWoman(const char* name) {
    return al::isEqualString(name, getShopNameCapWoman()) ||
           al::isEqualString(name, getShopNameSeaWoman()) ||
           al::isEqualString(name, getShopNameCityWoman());
}

bool ShopUtil::isShopCap(const char* name) {
    return al::isEqualString(name, getShopNameCapWoman()) ||
           al::isEqualString(name, getShopNameCapMan());
}

const char* ShopUtil::getWaitShopInActionName(const al::LiveActor* actor) {
    if (isShopCap(al::getModelName(actor)))
        return "WaitShop";
    return "WaitShopIn";
}

const char* ShopUtil::getWaitShopOutActionName(const al::LiveActor* actor) {
    if (isShopCap(al::getModelName(actor)))
        return "WaitShop";
    return "WaitShopOut";
}

const char* ShopUtil::getTalkShopInActionName(const al::LiveActor* actor) {
    if (isShopCap(al::getModelName(actor)))
        return "TalkShop";
    return "TalkShopIn";
}

const char* ShopUtil::getTalkShopOutActionName(const al::LiveActor* actor) {
    if (isShopCap(al::getModelName(actor)))
        return "TalkShop";
    return "TalkShopOut";
}

const char* ShopUtil::getWelcomeShopInActionName(const al::LiveActor* actor) {
    if (isShopCap(al::getModelName(actor)))
        return "Welcome";
    return "WelcomeIn";
}

const char* ShopUtil::getWelcomeShopOutActionName(const al::LiveActor* actor) {
    if (isShopCap(al::getModelName(actor)))
        return "Welcome";
    return "WelcomeOut";
}
