#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/HostIO/HioNode.h"
#include "Library/Scene/ISceneObj.h"

namespace al {
class ActorInitInfo;
class AudioDirector;
class IUseSceneObjHolder;
class MessageTagDataHolder;

struct NfpInfo;

}  // namespace al

class AmiiboNpcLayout;
class ProjectNfpDirector;
class SearchAmiiboDataTable;

class AmiiboNpcDirector : public al::ISceneObj, public al::IUseHioNode, public al::IUseAudioKeeper {
public:
    AmiiboNpcDirector();

    const char* getSceneObjName() const override;
    void init(ProjectNfpDirector* nfpDirector, al::AudioDirector* audioDirector);
    void initAfterPlacementSceneObj(const al::ActorInitInfo& initInfo) override;
    void updateSearchAmiiboName();
    bool requestAppearAmiiboLayout();
    void requestDecideAmiiboLayout();
    void requestEndAmiiboLayout();
    bool isEndAmiiboLayout();
    void registerSearchAmiibo(s32 id, s32 numberingId, u64 searchStartTime);
    void deleteSearchEndAmiibo();
    bool isSearchAmiibo(s32 id);
    u32 getSearchAmiiboNum() const;
    u32 getSearchEndAmiiboNum() const;
    u32 getSearchEndAmiiboNumRealTime() const;
    bool isEnableSearchAmiibo();
    void setTouchAmiiboName(s32 id, s32 numberingId);
    void trySetAmiiboCostumeName(s32 id);
    void checkTimeReverseAndRestore();
    al::NfpInfo* tryGetTriggerTouchNfpInfo();
    al::AudioKeeper* getAudioKeeper() const override;

private:
    AmiiboNpcLayout* mNpcLayout = nullptr;
    SearchAmiiboDataTable* mSearchDataTable = nullptr;
    ProjectNfpDirector* mNfpDirector = nullptr;
    al::NfpInfo* mNfpInfo = nullptr;
    al::AudioKeeper* mAudioKeeper = nullptr;
    al::MessageTagDataHolder* mTagDataHolder = nullptr;
    const char* mAmiiboNameCstr[3];
    sead::FixedSafeString<0x40> mAmiiboName[3];
    const char* mTouchAmiiboNameCstr = nullptr;
    sead::FixedSafeString<0x40> mTouchAmiiboName;
    const char16* mClothName = nullptr;
    const char16* mCapName = nullptr;
    u64 mTime = 0;
};

static_assert(sizeof(AmiiboNpcDirector) == 0x1D8, "AmiiboNpcDirector");

// Can't be split since these functions are in the middle of AmiiboNpcDirector in the executable

namespace AmiiboFunction {
al::NfpInfo* tryGetTriggerTouchNfpInfo(const al::IUseSceneObjHolder*);
al::NfpInfo* getLastTriggerTouchNfpInfo(const al::IUseSceneObjHolder*);
void startNfpTouch(const al::IUseSceneObjHolder*);
void stopNfpTouch(const al::IUseSceneObjHolder*);
bool isNfpErrorHandled(const al::IUseSceneObjHolder*);
bool requestAppearAmiiboLayout(const al::IUseSceneObjHolder*);
void requestDecideAmiiboLayout(const al::IUseSceneObjHolder*);
void requestEndAmiiboLayout(const al::IUseSceneObjHolder*);
bool isEndAmiiboLayout(const al::IUseSceneObjHolder*);
AmiiboNpcLayout* getAmiiboTouchLayout(const al::IUseSceneObjHolder*);
// These might return s32
u32 getSearchAmiiboNum(const al::IUseSceneObjHolder*);
u32 getSearchEndAmiiboNum(const al::IUseSceneObjHolder*);
u32 getSearchEndAmiiboNumRealTime(const al::IUseSceneObjHolder*);
void registerSearchAmiibo(const al::IUseSceneObjHolder*, const al::NfpInfo&);

bool isSearchAmiibo(const al::IUseSceneObjHolder*, const al::NfpInfo&);
void deleteSearchEndAmiibo(const al::IUseSceneObjHolder*);
void setTalkStartTime(const al::IUseSceneObjHolder*);
al::MessageTagDataHolder* getMessageTagDataHolder(const al::IUseSceneObjHolder*);
void setTouchAmiiboName(const al::IUseSceneObjHolder*, s32, s32);
void trySetAmiiboCostumeName(const al::IUseSceneObjHolder*, s32);
void tryCreateHelpAmiiboDirector(const al::IUseSceneObjHolder*);
bool isTriggerTouchAmiiboMario(const al::IUseSceneObjHolder*);
bool isTriggerTouchAmiiboPeach(const al::IUseSceneObjHolder*);
bool isTriggerTouchAmiiboKoopa(const al::IUseSceneObjHolder*);
bool isTriggerTouchAmiiboAll(const al::IUseSceneObjHolder*);

}  // namespace AmiiboFunction
