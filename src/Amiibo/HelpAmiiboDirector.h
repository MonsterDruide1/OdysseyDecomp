#pragma once

#include <container/seadPtrArray.h>

#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/HostIO/HioNode.h"
#include "Library/Nerve/IUseNerve.h"
#include "Library/Scene/ISceneObj.h"

namespace al {
struct NfpInfo;

struct ActorInitInfo;
class AudioDirector;
class AudioKeeper;
class IUseSceneObjHolder;
class LayoutActor;
class LayoutInitInfo;
class NerveKeeper;
class PlayerHolder;
class SimpleLayoutAppearWaitEnd;
}  // namespace al

class ProjectNfpDirector;
class HelpAmiiboCoinCollect;
class HelpAmiiboExecutor;

class HelpAmiiboDirector : public al::IUseHioNode,
                           public al::ISceneObj,
                           public al::IUseAudioKeeper,
                           public al::IUseNerve {
public:
    HelpAmiiboDirector();
    void init(ProjectNfpDirector* projectNfpDirector, const al::PlayerHolder* playerHolder,
              al::AudioDirector* audioDirector, const al::LayoutInitInfo& initInfo);
    void initAfterPlacementSceneObj(const al::ActorInitInfo& initInfo) override;

    bool isTriggerTouchAmiiboMario() const;
    bool isTriggerTouchAmiiboPeach() const;
    bool isTriggerTouchAmiiboKoopa() const;
    bool isTriggerTouchAmiiboYoshi() const;
    bool isTriggerTouchAmiiboAll() const;
    virtual void execute();
    void reset();
    bool isHelpAmiiboMode() const;
    void appearCoinCollectEffect();
    void killCoinCollectEffect();
    bool tryExecute(const al::NfpInfo* nfpInfo);

    const char* getSceneObjName() const override;
    al::AudioKeeper* getAudioKeeper() const override;
    al::NerveKeeper* getNerveKeeper() const override;

    void exeWait();
    void exeCountHold();
    void exeActive();
    void exeIconClose();

private:
    ProjectNfpDirector* mProjectNfpDirector = nullptr;
    const al::PlayerHolder* mPlayerHolder = nullptr;
    al::AudioKeeper* mAudioKeeper = nullptr;
    sead::PtrArray<HelpAmiiboExecutor> mTouchEntries;

    al::NerveKeeper* mNerveKeeper = nullptr;
    al::SimpleLayoutAppearWaitEnd* mSimpleLayout = nullptr;
    al::LayoutActor* mLayoutActor = nullptr;
    bool _58 = true;
    HelpAmiiboCoinCollect* mCoinCollect = nullptr;
};

namespace AmiiboFunction {

void tryCreateHelpAmiiboDirector(const al::IUseSceneObjHolder* objHolder);
bool isTriggerTouchAmiiboMario(const al::IUseSceneObjHolder* objHolder);
bool isTriggerTouchAmiiboPeach(const al::IUseSceneObjHolder* objHolder);
bool isTriggerTouchAmiiboKoopa(const al::IUseSceneObjHolder* objHolder);
bool isTriggerTouchAmiiboAll(const al::IUseSceneObjHolder* objHolder);

}  // namespace AmiiboFunction

namespace rs {

void resetHelpAmiiboState(const al::IUseSceneObjHolder* objHolder);
bool isHelpAmiiboMode(const al::IUseSceneObjHolder* objHolder);

}  // namespace rs
