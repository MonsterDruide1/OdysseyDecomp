#include "Util/AmiiboUtil.h"

#include "Library/Nfp/NfpFunction.h"
#include "Library/Nfp/NfpTypes.h"

namespace rs {

void calcSearchAmiiboBit(s32* gameId, s32* characterId, s32* characterVariant, s32 characterIdS32) {
    *gameId = (u8)(characterIdS32 >> 16);
    *characterId = (u8)(characterIdS32 >> 8);
    *characterVariant = (u8)characterIdS32;
}

s32 createCharacterIdS32(const al::NfpCharacterId& characterId) {
    return characterId.characterVariant + (characterId.characterId << 8) +
           (characterId.gameId << 16);
}

bool isEnableUseStageSceneAmiibo(const al::NfpInfo& nfpInfo) {
    return al::isCharacterIdBaseMario(nfpInfo) || al::isCharacterIdBasePeach(nfpInfo) ||
           al::isCharacterIdBaseKoopa(nfpInfo);
}

}  // namespace rs
