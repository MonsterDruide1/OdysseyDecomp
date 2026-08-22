#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class EventFlowChoiceInfo;
class LayoutInitInfo;
class LiveActor;
class MessageTagDataHolder;
class TalkMessageVoicePlayer;
}  // namespace al

class CommonSelectParts;

class TalkMessage : public al::LayoutActor {
public:
    TalkMessage(const char* name);

    void initLayoutTalk(const al::LayoutInitInfo& info, const char* suffix);
    void initLayoutWithArchiveName(const al::LayoutInitInfo& info, const char* archiveName,
                                   const char* suffix);
    void initLayoutImportant(const al::LayoutInitInfo& info, const char* suffix);
    void initLayoutOver(const al::LayoutInitInfo& info, const char* suffix);
    void initLayoutForEventTalk(const al::LayoutInitInfo& info);
    void initLayoutForEventImportant(const al::LayoutInitInfo& info);

    void startForNpc(const al::LiveActor* npcActor, const char16* message, const char16* name,
                     const al::MessageTagDataHolder* tagData, bool isWaitNextPage);
    void reset();
    void startForSystem(const char16* message, const al::MessageTagDataHolder* tagData,
                        bool isWaitNextPage);
    void end();
    bool isIconWait() const;
    void kill() override;
    bool isWait() const;

    void startSelectWithChoiceTable(const char16** choiceTable, s32 choiceNum, s32 selectedChoice);
    void startSelectWithChoiceInfo(const al::EventFlowChoiceInfo* choiceInfo);
    bool isSelectDecide() const;
    s32 getSelectedChoiceIndex() const;

    void exeAppear();
    void exeAppearWithText();
    void exeTextAnim();
    void exeIconAppearDelay();
    void exeIconAppear();
    void exeIconWait();
    void exeIconWaitTriggered();
    void exeIconPageNext();
    void exeIconPageNextAndPlayNextPage();
    void exeIconPageNextAndLoadNextMessage();
    void exeIconPageEnd();
    void exeWait();
    void exeEnd();

    void appear() override;
    void control() override;
    void startIconPageNext();

private:
    const al::LiveActor* mNpcActor = nullptr;
    al::TalkMessageVoicePlayer* mVoicePlayer = nullptr;
    al::LayoutActor* mParHardKey = nullptr;
    CommonSelectParts* mSelectParts = nullptr;
    const al::MessageTagDataHolder* mMessageTagDataHolder = nullptr;
    const char16* mMessage = nullptr;
    s32 mIconAppearDelay = -1;
    bool mIsWaitNextPage = false;
    bool mIsImportant = false;
    bool mIsEventTalk = false;
    bool mIsSkipSePlayed = false;
    bool mIsForceIcon = false;
    bool mIsLoadedNextPage = false;
};

static_assert(sizeof(TalkMessage) == 0x170);
