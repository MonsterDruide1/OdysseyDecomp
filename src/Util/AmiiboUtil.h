#pragma once

#include <basis/seadTypes.h>

#include "Library/Base/StringUtil.h"

namespace al {
struct NfpCharacterId;
struct NfpInfo;
class IUseMessageSystem;
class IUseSceneObjHolder;
}  // namespace al

namespace ShopItem {
struct ItemInfo;
}

class SearchAmiiboDataTable;

namespace rs {
void calcSearchAmiiboBit(s32* gameId, s32* characterId, s32* characterVariant, s32 characterIdS32);
s32 createCharacterIdS32(const al::NfpCharacterId& characterId);
bool isEnableUseStageSceneAmiibo(const al::NfpInfo& nfpInfo);
bool isExistAmiiboMstxtData(const al::IUseMessageSystem* user, const al::NfpInfo& nfpInfo);
al::StringTmp<64> getAmiiboMstxtLabel(bool*, const al::IUseMessageSystem* user, s32, s32, s32);
void tryFindAmiiboCostumeItemInfo(ShopItem::ItemInfo**, ShopItem::ItemInfo**,
                                  const al::NfpCharacterId& characterId, s32,
                                  al::IUseSceneObjHolder* user);
void resetHelpAmiibo(const al::IUseSceneObjHolder* user);
SearchAmiiboDataTable* getSearchAmiiboData(const al::IUseSceneObjHolder* user);
void appearCoinCollectHintEffect(const al::IUseSceneObjHolder* user);
void killCoinCollectHintEffect(const al::IUseSceneObjHolder* user);

}  // namespace rs
