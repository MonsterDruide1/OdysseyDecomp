#pragma once

#include <basis/seadTypes.h>

#include "Library/Base/StringUtil.h"

namespace al {
class IUseMessageSystem;
class LiveActor;
class SimpleLayoutAppearWaitEnd;
class TalkMessageVoicePlayer;
}  // namespace al

namespace ShopUtil {
void makeExplainLabel(al::StringTmp<256>* result, const char* firstString, const char* secondString,
                      bool isAddPerson);
void startNpcMessage(al::SimpleLayoutAppearWaitEnd* layout, const char16* msgString,
                     al::LiveActor* actor, const char* actorName, bool* something,
                     al::TalkMessageVoicePlayer* voicePlayer);
void startNpcMessage(al::SimpleLayoutAppearWaitEnd* layout, const char* messageName,
                     al::LiveActor* actor, const char* actorName, bool* something,
                     al::TalkMessageVoicePlayer* voicePlayer);
const char16* getSystemMessageStringForShop(const al::IUseMessageSystem* messageSystem,
                                            const char* messageName);
const char* getShopNameCityMan();
const char* getShopNameCityWoman();
const char* getShopNameSeaWoman();
const char* getShopNameCapWoman();
const char* getShopNameCapMan();
bool isShopWoman(const char* name);
bool isShopCap(const char* name);
const char* getWaitShopInActionName(const al::LiveActor* actor);
const char* getWaitShopOutActionName(const al::LiveActor* actor);
const char* getTalkShopInActionName(const al::LiveActor* actor);
const char* getTalkShopOutActionName(const al::LiveActor* actor);
const char* getWelcomeShopInActionName(const al::LiveActor* actor);
const char* getWelcomeShopOutActionName(const al::LiveActor* actor);
}  // namespace ShopUtil
