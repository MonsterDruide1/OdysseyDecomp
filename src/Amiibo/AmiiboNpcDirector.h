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
    void requestDecideAmiiboLayout();
    void requestEndAmiiboLayout();
    bool isEndAmiiboLayout();
    void registerSearchAmiibo(s32, s32, ulong);
    void deleteSearchEndAmiibo();
    bool isSearchAmiibo(s32);
    u32 getSearchAmiiboNum() const;
    u32 getSearchEndAmiiboNum() const;
    u32 getSearchEndAmiiboNumRealTime() const;
    bool isEnableSearchAmiibo();
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
const char* getMessageTagDataHolder(const al::IUseSceneObjHolder*);
void setTouchAmiiboName(const al::IUseSceneObjHolder*, s32, s32);
void trySetAmiiboCostumeName(const al::IUseSceneObjHolder*, s32);
void tryCreateHelpAmiiboDirector(const al::IUseSceneObjHolder*);
bool isTriggerTouchAmiiboMario(const al::IUseSceneObjHolder*);
bool isTriggerTouchAmiiboPeach(const al::IUseSceneObjHolder*);
bool isTriggerTouchAmiiboKoopa(const al::IUseSceneObjHolder*);
bool isTriggerTouchAmiiboAll(const al::IUseSceneObjHolder*);

}  // namespace AmiiboFunction
