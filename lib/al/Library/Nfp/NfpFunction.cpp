#include "Library/Nfp/NfpFunction.h"

#include "Library/Nfp/NfpTypes.h"

namespace al {

bool isAnyNfcTagDetected(const NfpInfo& nfpInfo) {
    return nfpInfo.isAmiibo || nfpInfo.isNormalNfc;
}

bool isNormalNfc(const NfpInfo& nfpInfo) {
    return nfpInfo.isNormalNfc;
}

bool isAmiibo(const NfpInfo& nfpInfo) {
    return nfpInfo.isAmiibo;
}

bool isEqualUniqueNfcId(const NfpInfo& nfpInfoA, const NfpInfo& nfpInfoB) {
    if (!nfpInfoA.isAmiibo || !nfpInfoB.isAmiibo)
        return false;

    return isEqualUniqueNfcId(nfpInfoA.tagInfo, nfpInfoB.tagInfo);
}

bool isEqualUniqueNfcId(const nn::nfp::TagInfo& tagInfoA, const nn::nfp::TagInfo& tagInfoB) {
    return isEqualUniqueNfcId(tagInfoA.id, tagInfoB.id);
}

bool isEqualCharacterId(const NfpInfo& nfpInfo, NfpCharacterId characterId) {
    if (!nfpInfo.isAmiibo)
        return false;

    return isEqualCharacterId(nfpInfo.modelInfo, characterId);
}

bool isEqualCharacterId(const nn::nfp::ModelInfo& modelInfo, const NfpCharacterId& characterId) {
    return isEqualCharacterIdBase(modelInfo, characterId) &&
           characterId.characterVariant == modelInfo.characterVariant;
}

bool isEqualCharacterIdBase(const NfpInfo& nfpInfo, NfpCharacterId characterId) {
    if (!nfpInfo.isAmiibo)
        return false;

    return isEqualCharacterId(nfpInfo.modelInfo, characterId);
}

bool isEqualNumberingId(const NfpInfo& nfpInfo, s32 numberingId) {
    if (!nfpInfo.isAmiibo)
        return false;

    return isEqualNumberingId(nfpInfo.modelInfo, numberingId);
}

bool isEqualNumberingId(const nn::nfp::ModelInfo& modelInfo, s32 numberingId) {
    return modelInfo.modelNumber == numberingId;
}

bool tryGetCharacterId(NfpCharacterId* characterId, const NfpInfo& nfpInfo) {
    if (!nfpInfo.isAmiibo)
        return false;

    characterId->gameId = nfpInfo.modelInfo.gameId;
    characterId->characterId = nfpInfo.modelInfo.characterId;
    characterId->characterVariant = nfpInfo.modelInfo.characterVariant;
    return true;
}

bool tryGetNumberingId(s32* numberingId, const NfpInfo& nfpInfo) {
    if (!nfpInfo.isAmiibo)
        return false;

    *numberingId = nfpInfo.modelInfo.modelNumber;
    return true;
}

bool isCharacterIdBaseMario(const NfpInfo& nfpInfo) {
    return isEqualCharacterIdBase(nfpInfo, {0x00, 0x00, 0x00});
}

bool isCharacterIdBaseDrMario(const NfpInfo& nfpInfo) {
    return isEqualCharacterIdBase(nfpInfo, {0x00, 0x00, 0x01});
}

bool isCharacterIdBasePeach(const NfpInfo& nfpInfo) {
    return isEqualCharacterIdBase(nfpInfo, {0x00, 0x02, 0x00});
}

bool isCharacterIdBaseKoopa(const NfpInfo& nfpInfo) {
    return isEqualCharacterIdBase(nfpInfo, {0x00, 0x05, 0x00});
}

bool isCharacterIdBaseYoshi(const NfpInfo& nfpInfo) {
    return isEqualCharacterIdBase(nfpInfo, {0x00, 0x03, 0x00});
}

bool isCharacterIdBaseKinopio(const NfpInfo& nfpInfo) {
    return isEqualCharacterIdBase(nfpInfo, {0x00, 0x0a, 0x00});
}

bool isNeedFormat(const NfpInfo& nfpInfo) {
    return nfpInfo.isNeedFormat;
}

bool isNeedRestore(const NfpInfo& nfpInfo) {
    return nfpInfo.isNeedRestore;
}

bool isNeedRegister(const NfpInfo& nfpInfo) {
    return nfpInfo.isNeedRegister;
}

bool isNeedRegisterNickName(const NfpInfo& nfpInfo) {
    return (char)nfpInfo.nickName[0] == '\0';
}

bool isValidRegisterInfo(const NfpInfo& nfpInfo) {
    return false;
}

void getAmiiboOwnerName(sead::BufferedSafeStringBase<char16>* ownerName, const NfpInfo& nfpInfo) {
    // Stubbed in game
}

void getAmiiboNickName(sead::BufferedSafeStringBase<char16>* nickName, const NfpInfo& nfpInfo) {
    nickName->copy(nfpInfo.nickName);
}

bool isInvalidFormatVersion(const NfpInfo& nfpInfo) {
    if (!nfpInfo.isFormatVersionSet)
        return false;

    if (nfpInfo.formatVersion.minor != 0x73)
        return false;

    return nfpInfo.formatVersion.major == 0xb8;
}

bool isEqualCharacterIdBase(const nn::nfp::ModelInfo& modelInfo,
                            const NfpCharacterId& characterId) {
    if (characterId.gameId != modelInfo.gameId)
        return false;

    return characterId.characterId == modelInfo.characterId;
}

bool isEqualUniqueNfcId(const nn::nfp::TagInfo& tagInfoA, const NfpInfo& nfpInfoB) {
    if (!nfpInfoB.isAmiibo)
        return false;

    return isEqualUniqueNfcId(tagInfoA, nfpInfoB.tagInfo);
}

bool isEqualUniqueNfcId(const nn::nfp::TagId& tagIdA, const nn::nfp::TagId& tagIdB) {
    if (tagIdA.uuidLength != tagIdB.uuidLength)
        return false;

    for (s32 i = 0; i < tagIdA.uuidLength; i++)
        if (tagIdA.uuid[i] != tagIdB.uuid[i])
            return false;

    return true;
}

}  // namespace al
