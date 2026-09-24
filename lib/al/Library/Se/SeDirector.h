#pragma once

#include <basis/seadTypes.h>

#include "Library/Audio/System/AudioSystemInfo.h"
#include "Library/Se/SeDirectorInitInfo.h"

namespace al {
class AudioDirector;
struct SeDirectorInitInfo;
struct AudioSystemInfo;
class IUseActiveBgmLine;
class SePlayerGroupKeeper;
class SeListenerKeeper;
class MeInfoKeeper;
class SeMaterialInfoKeeper;
class SeCategoryHolder;
class SeSituationKeeper;
class SeStageInfo;
class SeSourceKeeper;
class SeBarrierKeeper;
class SeLoopSequencer;
class SeDuckingController;
class SeDataBase;
class SeKeeper;
class PadRumbleDirector;
class SeListener;
class AreaObjDirector;
class PlayerHolder;
class SeRequestParam;
class SeEmitter;
class AudioBusSendController;
class MeInfo;

class SeDirector {
public:
    SeDirector();

    void init(const AudioDirector* audioDirector, const SeDirectorInitInfo& initInfo,
              const char* stageName, s32 scenarioNo, const AudioSystemInfo* audioSystemInfo,
              MeInfoKeeper* meInfoKeeper, const IUseActiveBgmLine* activeBgmLineUser);
    void init3D(const SeDirectorInitInfo& initInfo);
    void initAfterInitPlacement(AreaObjDirector* areaObjDirector);
    void finalize();
    void update();

    void notifyIsModeHandheld(bool inHandheld);
    void addRequest(SeRequestParam* requestParam, const char*, s32, bool);
    void notifiedUpdateMaterial(SeEmitter* emitter, const char*, s32);
    void pauseSystem(bool, const char* pauseReason, u32);
    void startSituation(const char* situationName, s32, s32, s32);
    void endSituation(const char* situationName, s32);
    void checkIsActiveSituation(const char* situationName) const;
    void requestPlayLoopSeSequence(const char*, const MeInfo*, s32);

    void setPlayerHolder(const PlayerHolder* playerHolder);
    void setAudioBusSendController(AudioBusSendController* audioBusSendController);
    SeListener* getListener(s32 index) const;
    SeDataBase* getSeDataBase();
    bool isSystemPaused() const;
    f32 getDuckingVolume() const;
    const char* getStageEffectName() const;

    void setUpperLevelKeeper(SeKeeper* keeper) { mUpperLevelKeeper = keeper; }

private:
    SePlayerGroupKeeper* mPlayerGroupKeeper;
    SeListenerKeeper* mListenerKeeper;
    MeInfoKeeper* mMeInfoKeeper;
    SeMaterialInfoKeeper* mSeMaterialInfoKeeper;
    s32 mListenerCount;
    SeCategoryHolder* mCategoryHolder;
    SeSituationKeeper* mSituationKeeper;
    SeStageInfo* mStageInfo;
    const char* mStageEffectName;
    SeSourceKeeper* mSourceKeeper;
    SeBarrierKeeper* mBarrierKeeper;
    SeLoopSequencer* mLoopSequencer;
    SeDuckingController* mDuckingController;
    SeDataBase* mDatabase;
    void* _70;
    f32 mVolume;
    SeKeeper* mUpperLevelKeeper;
    const char** mPauseReasons;
    s32 mPauseReasonCount;
    bool* _98;  // true when less than 3 sound channels are available
    bool* mIsStereoOutput;
    const char** _a8;
    s32 _b0;
    PadRumbleDirector* mPadRumbleDirector;
};

static_assert(sizeof(SeDirector) == 0xc0);
}  // namespace al
