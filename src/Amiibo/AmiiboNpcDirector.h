#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/HostIO/HioNode.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
struct NfpInfo;

class ActorInitInfo;
class AudioDirector;
class IUseSceneObjHolder;
class MessageTagDataHolder;
}  // namespace al

class AmiiboNpcLayout;
class ProjectNfpDirector;
class SearchAmiiboDataTable;

class AmiiboNpcDirector : public al::ISceneObj, public al::IUseHioNode, public al::IUseAudioKeeper {
public:
    static constexpr s32 sSceneObjId = SceneObjID_AmiiboNpcDirector;

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

    AmiiboNpcLayout* getAmiiboNpcLayout() const { return mNpcLayout; }

    ProjectNfpDirector* getProjectNfpDirector() const { return mNfpDirector; }

    al::NfpInfo* getNfpInfo() const { return mNfpInfo; }

    al::MessageTagDataHolder* getMessageTagDataHolder() const { return mTagDataHolder; }

    void setTime(u64 time) { mTime = time; }

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

namespace AmiiboFunction {

al::NfpInfo* tryGetTriggerTouchNfpInfo(const al::IUseSceneObjHolder* objHolder);
al::NfpInfo* getLastTriggerTouchNfpInfo(const al::IUseSceneObjHolder* objHolder);
void startNfpTouch(const al::IUseSceneObjHolder* objHolder);
void stopNfpTouch(const al::IUseSceneObjHolder* objHolder);
bool isNfpErrorHandled(const al::IUseSceneObjHolder* objHolder);
bool requestAppearAmiiboLayout(const al::IUseSceneObjHolder* objHolder);
void requestDecideAmiiboLayout(const al::IUseSceneObjHolder* objHolder);
void requestEndAmiiboLayout(const al::IUseSceneObjHolder* objHolder);
bool isEndAmiiboLayout(const al::IUseSceneObjHolder* objHolder);
AmiiboNpcLayout* getAmiiboTouchLayout(const al::IUseSceneObjHolder* objHolder);
u32 getSearchAmiiboNum(const al::IUseSceneObjHolder* objHolder);
u32 getSearchEndAmiiboNum(const al::IUseSceneObjHolder* objHolder);
u32 getSearchEndAmiiboNumRealTime(const al::IUseSceneObjHolder* objHolder);
void registerSearchAmiibo(const al::IUseSceneObjHolder* objHolder, const al::NfpInfo& nfpInfo);
bool isSearchAmiibo(const al::IUseSceneObjHolder* objHolder, const al::NfpInfo& nfpInfo);
void deleteSearchEndAmiibo(const al::IUseSceneObjHolder* objHolder);
void setTalkStartTime(const al::IUseSceneObjHolder* objHolder);
al::MessageTagDataHolder* getMessageTagDataHolder(const al::IUseSceneObjHolder* objHolder);
void setTouchAmiiboName(const al::IUseSceneObjHolder* objHolder, s32 id, s32 numberingId);
void trySetAmiiboCostumeName(const al::IUseSceneObjHolder* objHolder, s32 id);

}  // namespace AmiiboFunction
