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
bool isAnyNfcTagDetected(const NfpInfo&);
bool isNormalNfc(const NfpInfo&);
bool isAmiibo(const NfpInfo&);
bool isEqualUniqueNfcId(const NfpInfo&, const NfpInfo&);
bool isEqualUniqueNfcId(const nn::nfp::TagInfo&, const nn::nfp::TagInfo&);
bool isEqualCharacterId(const NfpInfo&, NfpCharacterId);
bool isEqualCharacterId(const nn::nfp::ModelInfo&, const NfpCharacterId&);
bool isEqualCharacterIdBase(const NfpInfo&, NfpCharacterId);
bool isEqualNumberingId(const NfpInfo&, s32);
bool isEqualNumberingId(const nn::nfp::ModelInfo&, s32);
bool tryGetCharacterId(NfpCharacterId*, const NfpInfo&);
bool tryGetNumberingId(s32*, const NfpInfo&);
bool isCharacterIdBaseMario(const NfpInfo&);
bool isCharacterIdBaseDrMario(const NfpInfo&);
bool isCharacterIdBasePeach(const NfpInfo&);
bool isCharacterIdBaseKoopa(const NfpInfo&);
bool isCharacterIdBaseYoshi(const NfpInfo&);
bool isCharacterIdBaseKinopio(const NfpInfo&);
bool isNeedFormat(const NfpInfo&);
bool isNeedRestore(const NfpInfo&);
bool isNeedRegister(const NfpInfo&);
bool isNeedRegisterNickName(const NfpInfo&);
bool isValidRegisterInfo(const NfpInfo&);
void getAmiiboOwnerName(sead::BufferedSafeStringBase<char16>*, const NfpInfo&);
void getAmiiboNickName(sead::BufferedSafeStringBase<char16>*, const NfpInfo&);
bool isInvalidFormatVersion(const NfpInfo&);
bool isEqualCharacterIdBase(const nn::nfp::ModelInfo&, const NfpCharacterId&);
bool isEqualUniqueNfcId(const nn::nfp::TagInfo&, const NfpInfo&);
bool isEqualUniqueNfcId(const nn::nfp::TagId&, const nn::nfp::TagId&);

}  // namespace al
