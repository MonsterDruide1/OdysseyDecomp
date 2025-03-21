#pragma once

#include "Library/Message/IUseMessageSystem.h"
#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LayoutInitInfo;
class Scene;
class SimpleLayoutAppearWaitEnd;
class MessageSystem;
class WindowConfirm;
}  // namespace al
class CommonVerticalList;
class FooterParts;
class GameConfigData;
class GameDataHolder;
class InputSeparator;
class SimpleLayoutMenu;
class WindowConfirmData;

class StageSceneStateOption : public al::HostStateBase<al::Scene>, public al::IUseMessageSystem {
public:
    StageSceneStateOption(const char*, al::Scene*, const al::LayoutInitInfo&, FooterParts*,
                          GameDataHolder*, bool);

    void updateConfigDataInfo(const GameConfigData*);
    void killAllLayouts();
    void init() override;
    void appear() override;
    void kill() override;
    bool isModeSelectEnd() const;
    s32 getSelectedFileId() const;
    bool isChangeLanguage() const;
    void exeOptionTop();
    void decide(const al::Nerve*, SimpleLayoutMenu*, CommonVerticalList*);
    void exeModeSelectSelecting();
    void openConfirm(const al::Nerve*, SimpleLayoutMenu*, CommonVerticalList*);
    void cancel(const al::Nerve*, SimpleLayoutMenu*, CommonVerticalList*);
    void exeModeSelectSelectingByHelp();
    void exeModeSelectConfirmYesNo();
    void exeModeSelectConfirmEnd();
    void exeConfig();
    void endConfig();
    void exeDataManager();
    void exeSaveDataSelecting();
    void updateSaveDataInfo(bool);
    void exeSaveDataConfirmYesNo();
    void exeSaveDataSaving();
    void exeSaveDataSaved();
    void exeLoadDataSelecting();
    void exeLoadDataConfirmNg();
    void exeLoadDataConfirmYesNo();
    void exeLoadDataSaving();
    void exeDeleteDataSelecting();
    void exeDeleteDataConfirmNg();
    void exeDeleteDataConfirmYesNo();
    void exeDeleteDataDeleting();
    void exeDeleteDataDeleted();
    void exeLanguageSetting();
    void exeLanguageSettingConfirmYesNo();
    void exeWaitEndDecideAnim();
    void exeWaitEndDecideAnimAndAutoSave();
    void exeWaitEndAutoSave();
    void exeClose();
    void changeNerve(const al::Nerve*, SimpleLayoutMenu*, CommonVerticalList*);

    const al::MessageSystem* getMessageSystem() const override;

    const char* getLanguage() const { return mLanguage; };

    void set_51(bool isTrue) { field_51 = isTrue; }

    bool isLoadData() const { return mIsLoadData; }

private:
    void* field_28;
    void* field_30;
    void* field_38;
    char* field_40;
    char* field_48;
    bool field_50;
    bool field_51;
    FooterParts* mFooterParts;
    void* field_60;
    SimpleLayoutMenu* field_68;
    CommonVerticalList* field_70;
    al::SimpleLayoutAppearWaitEnd* field_78;
    CommonVerticalList* field_80;
    SimpleLayoutMenu* field_88;
    CommonVerticalList* field_90;
    al::SimpleLayoutAppearWaitEnd* field_98;
    FooterParts* field_a0;
    SimpleLayoutMenu* field_a8;
    CommonVerticalList* mCtrlSettingsList;
    SimpleLayoutMenu* field_b8;
    CommonVerticalList* field_c0;
    SimpleLayoutMenu* field_c8;
    void* field_d0;
    WindowConfirmData* field_d8;
    SimpleLayoutMenu* field_e0;
    CommonVerticalList* field_e8;
    void* field_f0;
    void* field_f8;
    void* field_100;
    void* field_108;
    void* field_110;
    void* field_118;
    void* field_120;
    void* field_128;
    void* field_130;
    void* field_138;
    s32 field_140;
    s32 field_144;
    SimpleLayoutMenu* field_148;
    FooterParts* field_150;
    CommonVerticalList* field_158;
    al::WindowConfirm* field_160;
    const char* mLanguage;
    al::Scene* mScene;
    GameDataHolder* mGameDataHolder;
    bool mIsLoadData;
    al::MessageSystem* mMessageSystem;
    InputSeparator* mInputSeperator;
};
