#pragma once

#include <prim/seadSafeString.h>

namespace nn {
class Result;
}  // namespace nn

namespace nn::nfp {
struct ModelInfo;
struct TagId;
struct TagInfo;
}  // namespace nn::nfp

namespace al {
struct NfpCharacterId;
class NfpDirector;
struct NfpInfo;

bool isNfpTriggerTouch(NfpDirector*, s32);
bool tryGetTriggerTouchNfpInfo(NfpDirector*, s32);
bool isNfpError(NfpDirector*, s32);
bool isNfpErrorDisableNfcDevice(const nn::Result&);
bool isNfpErrorBreakTagDisableRestore(const nn::Result&);
bool isNfpErrorBreakTagEnableRestore(const nn::Result&);
void showNfpErrorApplet(const nn::Result&);
bool isAnyNfcTagDetected(const NfpInfo& nfpInfo);
bool isNormalNfc(const NfpInfo& nfpInfo);
bool isAmiibo(const NfpInfo& nfpInfo);
bool isEqualUniqueNfcId(const NfpInfo& nfpInfoA, const NfpInfo& nfpInfoB);
bool isEqualUniqueNfcId(const nn::nfp::TagInfo& tagInfoA, const nn::nfp::TagInfo& tagInfoB);
bool isEqualCharacterId(const NfpInfo& nfpInfo, NfpCharacterId characterId);
bool isEqualCharacterId(const nn::nfp::ModelInfo& modelInfo, const NfpCharacterId& characterId);
bool isEqualCharacterIdBase(const NfpInfo& nfpInfo, NfpCharacterId characterId);
bool isEqualNumberingId(const NfpInfo& nfpInfo, s32 numberingId);
bool isEqualNumberingId(const nn::nfp::ModelInfo& modelInfo, s32 numberingId);
bool tryGetCharacterId(NfpCharacterId* characterId, const NfpInfo& nfpInfo);
bool tryGetNumberingId(s32* numberingId, const NfpInfo& nfpInfo);
bool isCharacterIdBaseMario(const NfpInfo& nfpInfo);
bool isCharacterIdBaseDrMario(const NfpInfo& nfpInfo);
bool isCharacterIdBasePeach(const NfpInfo& nfpInfo);
bool isCharacterIdBaseKoopa(const NfpInfo& nfpInfo);
bool isCharacterIdBaseYoshi(const NfpInfo& nfpInfo);
bool isCharacterIdBaseKinopio(const NfpInfo& nfpInfo);
bool isNeedFormat(const NfpInfo& nfpInfo);
bool isNeedRestore(const NfpInfo& nfpInfo);
bool isNeedRegister(const NfpInfo& nfpInfo);
bool isNeedRegisterNickName(const NfpInfo& nfpInfo);
bool isValidRegisterInfo(const NfpInfo& nfpInfo);
void getAmiiboOwnerName(sead::BufferedSafeStringBase<char16>* ownerName, const NfpInfo& nfpInfo);
void getAmiiboNickName(sead::BufferedSafeStringBase<char16>* nickName, const NfpInfo& nfpInfo);
bool isInvalidFormatVersion(const NfpInfo& nfpInfo);
bool isEqualCharacterIdBase(const nn::nfp::ModelInfo& modelInfo, const NfpCharacterId& characterId);
bool isEqualUniqueNfcId(const nn::nfp::TagInfo& tagInfoA, const NfpInfo& nfpInfoB);
bool isEqualUniqueNfcId(const nn::nfp::TagId& tagIdA, const nn::nfp::TagId& tagIdB);

}  // namespace al
