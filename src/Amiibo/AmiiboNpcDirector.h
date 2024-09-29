#pragma once

#include <basis/seadTypes.h>

#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/HostIO/HioNode.h"
#include "Library/Scene/ISceneObj.h"

namespace al {
class IUseSceneObjHolder;
class NfpInfo;
class ActorInitInfo;
class AudioDirector;
}  // namespace al

class AmiiboNpcLayout;
class ProjectNfpDirector;

class AmiiboNpcDirector : public al::ISceneObj, public al::IUseHioNode, public al::IUseAudioKeeper {
public:
    AmiiboNpcDirector();
    const char* getSceneObjName() const override;
    virtual ~AmiiboNpcDirector();
    void init(ProjectNfpDirector*, al::AudioDirector*);
    void initAfterPlacementSceneObj(const al::ActorInitInfo&) override;
    void updateSearchAmiiboName();
    bool requestAppearAmiiboLayout();
    bool requestDecideAmiiboLayout();
    bool requestEndAmiiboLayout();
    bool isEndAmiiboLayout();
    u32 registerSearchAmiibo(s32, s32, ulong);
    bool deleteSearchEndAmiibo();
    bool isSearchAmiibo(s32);
    u32 getSearchAmiiboNum();
    u32 getSearchEndAmiiboNum();
    u32 getSearchEndAmiiboNumRealTime();
    u32 isEnableSearchAmiibo();
    void setTouchAmiiboName(s32, s32);
    void trySetAmiiboCostumeName(s32);
    void checkTimeReverseAndRestore();
    al::NfpInfo* tryGetTriggerTouchNfpInfo();
    al::AudioKeeper* getAudioKeeper() const override;

private:
    char filler[0x1C8];
};

static_assert(sizeof(AmiiboNpcDirector) == 0x1D8, "AmiiboNpcDirector");

// Can't be split since these functions are in the middle of AmiiboNpcDirector in the executable

class AmiiboFunction {
public:
    static al::NfpInfo* tryGetTriggerTouchNfpInfo(const al::IUseSceneObjHolder*);
    static al::NfpInfo* getLastTriggerTouchNfpInfo(const al::IUseSceneObjHolder*);
    static void startNfpTouch(const al::IUseSceneObjHolder*);
    static void stopNfpTouch(const al::IUseSceneObjHolder*);
    static bool isNfpErrorHandled(const al::IUseSceneObjHolder*);
    static bool requestAppearAmiiboLayout(const al::IUseSceneObjHolder*);
    static bool requestDecideAmiiboLayout(const al::IUseSceneObjHolder*);
    static bool requestEndAmiiboLayout(const al::IUseSceneObjHolder*);
    static bool isEndAmiiboLayout(const al::IUseSceneObjHolder*);
    static AmiiboNpcLayout* getAmiiboTouchLayout(const al::IUseSceneObjHolder*);
    // These might return s32
    static u32 getSearchAmiiboNum(const al::IUseSceneObjHolder*);
    static u32 getSearchEndAmiiboNum(const al::IUseSceneObjHolder*);
    static u32 getSearchEndAmiiboNumRealTime(const al::IUseSceneObjHolder*);
    static u32 registerSearchAmiibo(const al::IUseSceneObjHolder*, const al::NfpInfo&);

    static bool isSearchAmiibo(const al::IUseSceneObjHolder*, const al::NfpInfo&);
    static bool deleteSearchEndAmiibo(const al::IUseSceneObjHolder*);
    static void setTalkStartTime(const al::IUseSceneObjHolder*);
    static const char* getMessageTagDataHolder(const al::IUseSceneObjHolder*);
    static void setTouchAmiiboName(const al::IUseSceneObjHolder*, s32, s32);
    static void trySetAmiiboCostumeName(const al::IUseSceneObjHolder*, s32);
};
