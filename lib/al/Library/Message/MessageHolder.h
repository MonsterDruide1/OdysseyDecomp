#pragma once

#include <prim/seadSafeString.h>

#include "Library/HostIO/HioNode.h"

namespace al {

class IUseMessageSystem;
class LayoutActor;
class LayoutSystem;
class MessageProjectEx;
class MessageTag;
class MessageTagDataHolder;

struct ReplaceTimeInfo {
    u32 year;
    u32 month;
    u32 day;
    u32 hour;
    u32 minute;
    u32 second;
    u32 centiSecond;
};

class MessageHolder : public HioNode {
public:
    MessageHolder();

    void init(const char*, const char*);
    const char16* getText(s32) const;
    const char16* getText(const char*) const;
    const char16* tryGetText(const char*) const;
    bool isExistText(const char*) const;
    s32 calcCharacterNum(s32) const;
    s32 calcCharacterNum(const char*) const;
    s32 calcCharacterByteSize(const char*) const;
    s32 getTextNum() const;
    void searchTextLabelByIndex(sead::BufferedSafeString*, s32) const;
    s32 getStyleByIndex(s32) const;
    s32 trySearchStyleIndexByLabel(const char*) const;
};

const char* getLanguage();
const char* getLayoutMessageArcName();
bool isMessageTagMark(char16);
bool isMessageTagEndMark(char16);
bool isMessageTagPageBreak(const IUseMessageSystem*, const char16*);
bool isMessageTagPageBreak(const IUseMessageSystem*, const MessageTag&);
bool isMessageTagPageBreak(const MessageProjectEx*, const MessageTag&);
const char* getMessageTagGroupName(const MessageProjectEx*, s32);
const char* getMessageTagGroupName(const IUseMessageSystem*, s32);
const char* getMessageTagName(const MessageProjectEx*, s32, s32);
const char* getMessageTagName(const IUseMessageSystem*, s32, s32);
bool isExistMessageTag(const char16*);
s32 calcMessageSizeWithoutNullCharacter(const char16*, const char16*);
bool isExistMessageTagTextPaneAnim(const IUseMessageSystem*, const char16*);
bool tryGetMessageTagTextAnim(sead::BufferedSafeString*, const IUseMessageSystem*, const char16*);
bool isMessageTagVoice(const IUseMessageSystem*, const char16*);
void getMessageTagVoiceName(sead::BufferedSafeString*, const IUseMessageSystem*, const char16*);
bool tryGetMessageTagVoiceNameInPage(sead::BufferedSafeString*, const IUseMessageSystem*,
                                     const char16*);
bool isMessageTagPictFont(const IUseMessageSystem*, s32);
bool isMessageTagDeviceFont(const IUseMessageSystem*, s32);
bool isExistMessageTagPadSwitch(const IUseMessageSystem*, const char16*, s32);
bool isMessageTagPadStyle(const IUseMessageSystem*, s32, s32);
bool isMessageTagPadPair(const IUseMessageSystem*, s32, s32);
bool isMessageTagPadStyle2P(const IUseMessageSystem*, s32, s32);
bool isMessageTagAlignLeft(const IUseMessageSystem*, s32, s32);
bool isMessageTagAlignCenter(const IUseMessageSystem*, s32, s32);
void replaceMessageTagString(sead::WBufferedSafeString*, const IUseMessageSystem*, const char16*,
                             const char16*);
void replaceMessageTagTimeDirectRaceTime(sead::WBufferedSafeString*, const IUseMessageSystem*,
                                         ReplaceTimeInfo&);
const char16* getSystemMessageString(const IUseMessageSystem*, const char*, const char*);
void replaceMessageTagTimeDirectDate(sead::WBufferedSafeString*, const IUseMessageSystem*,
                                     ReplaceTimeInfo&);
void replaceMessageTagTimeDirectDateDetail(sead::WBufferedSafeString*, const IUseMessageSystem*,
                                           ReplaceTimeInfo&);
void replaceMessageTagScore(sead::WBufferedSafeString*, const IUseMessageSystem*, const char16*,
                            s32, const char*);
void replaceMessageTagCoinNum(sead::WBufferedSafeString*, const IUseMessageSystem*, const char16*,
                              s32, const char*);
void replaceMessageTagAmiiboName(sead::WBufferedSafeString*, const IUseMessageSystem*,
                                 const char16*, const char*, const char*);
void replaceMessageTagUserName(sead::WBufferedSafeString*, const IUseMessageSystem*, const char16*,
                               const char16*, const char*);
void replaceMessageTagNamedString(sead::WBufferedSafeString*, const IUseMessageSystem*,
                                  const char16*, const char16*, const char*);
void replaceMessageTagTime(sead::WBufferedSafeString*, const IUseMessageSystem*, const char16*,
                           ReplaceTimeInfo&, const char*);
void createReplaceTimeInfoForRaceTime(ReplaceTimeInfo*, s32, s32, s32);
void createReplaceTimeInfoForDateTime(ReplaceTimeInfo*, u64);
void replacePaneDateTime(LayoutActor*, const char*, u64);
MessageTagDataHolder* initMessageTagDataHolder(s32);
void registerMessageTagDataScore(MessageTagDataHolder*, const char*, const s32*);
void registerMessageTagDataCoinNum(MessageTagDataHolder*, const char*, const s32*);
void registerMessageTagDataUserName(MessageTagDataHolder*, const char*, const char16**);
void registerMessageTagDataAmiiboName(MessageTagDataHolder*, const char*, const char**);
void registerMessageTagDataString(MessageTagDataHolder*, const char*, const char16**);
void replaceMessageTagData(sead::WBufferedSafeString*, const IUseMessageSystem*,
                           const MessageTagDataHolder*, const char16*);
s32 calcMessageSizeWithoutTag(const char16*, const char16*);
bool isExistLayoutMessage(const IUseMessageSystem*, const char*);
bool isExistSystemMessage(const IUseMessageSystem*, const char*);
bool isExistStageMessage(const IUseMessageSystem*, const char*);
bool isExistLabelInLayoutMessage(const IUseMessageSystem*, const char*, const char*);
bool isExistLabelInSystemMessage(const IUseMessageSystem*, const char*, const char*);
bool isExistLabelInStageMessage(const IUseMessageSystem*, const char*, const char*);
s32 calcSystemMessageCharacterNum(const IUseMessageSystem*, const char*, const char*);
s32 calcSystemMessageCharacterNumWithoutTag(const IUseMessageSystem*, const char*, const char*);
const char16* getLayoutMessageString(const IUseMessageSystem*, const char*, const char*);
const char16* getStageMessageString(const IUseMessageSystem*, const char*, const char*);
bool tryGetStageMessageString(const char16**, const IUseMessageSystem*, const char*, const char*);
const char16* getLayoutMessageString(const IUseMessageSystem*, const char*, s32);
const char16* getSystemMessageString(const IUseMessageSystem*, const char*, s32);
s32 getSystemMessageLabelNum(const IUseMessageSystem*, const char*);
s32 getLayoutMessageLabelNum(const IUseMessageSystem*, const char*);
const char* getMessageTagParamName(const IUseMessageSystem*, const MessageTag&, s32);
s32 getMessageTagParamNum(const IUseMessageSystem*, const MessageTag&);
void getMessageTagParamString(sead::WBufferedSafeString*, const IUseMessageSystem*,
                              const MessageTag&, s32);
void getSystemMessageLabelString(sead::BufferedSafeString*, const IUseMessageSystem*, const char*,
                                 s32);
void getLayoutMessageLabelString(sead::BufferedSafeString*, const IUseMessageSystem*, const char*,
                                 s32);
void getMessageLine(char16*, u32, const char16*, u32);
s32 countMessageLine(const char16*);
bool copyMessageExpandTag(char16*, s32, const IUseMessageSystem*, const char16*);
bool copyMessageWithoutTag(char16*, s32, const char16*);
bool copyMessageWithoutRubyTag(char16*, s32, const IUseMessageSystem*, const char16*);
void copyMessageWithoutTagExpandRuby(char16*, s32, const IUseMessageSystem*, const char16*);
s32 copyMessageWithTag(char16*, s32, const char16*);
s32 copyMessageWithTagOnlyCurrentPage(const IUseMessageSystem*, char16*, s32, const char16*, s32);
const char16* getMessageWithPage(const IUseMessageSystem*, const char16*, s32);
void copyMessageOnlyRuby(char16*, s32, const IUseMessageSystem*, const char16*);
s32 countMessagePage(const IUseMessageSystem*, const char16*, s32);
const char16* getNextMessagePage(const IUseMessageSystem*, const char16*);
void* getPictureFont(const LayoutSystem*);  // TODO: unknown return type

}  // namespace al
