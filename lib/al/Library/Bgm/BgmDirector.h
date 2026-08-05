#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

namespace al {
class AudioBusSendController;
class AudioDirector;
struct AudioSystemInfo;
class Bgm3DParamsController;
class BgmDataBase;
class BgmDirectorInitInfo;
class BgmLine;
class BgmLineKeeper;
class BgmMultiPlayingController;
class BgmRhythmSyncDirector;
class BgmSituationDirector;
struct BgmPlayingRequest;
class FunctorBase;
class IBgmParamsChanger;

class BgmDirector {
public:
    BgmDirector();

    virtual BgmLine* getActiveBgmLine() const;

    void init(const AudioSystemInfo* audioSystemInfo, const AudioDirector* audioDirector,
              const BgmDirectorInitInfo& initInfo, const char* userName, s32 lineNum);
    void setDependentModule(BgmDirector* director);
    void reset(const char* userName, s32 lineNum);
    void update();
    void clearPlayData();
    void enableLineChange();
    void finalize();
    void startBgm(const BgmPlayingRequest& request, bool isForceStart);
    void prepareBgm(const BgmPlayingRequest& request);
    void stopBgm(const char* playName, s32 fadeFrame);
    void endSituation(const char* situationName, bool isApply, bool isForce);
    void pauseBgm(const char* playName, s32 fadeFrame);
    void resumeBgm(const char* playName, s32 fadeFrame);
    bool isPlayingBgm();
    bool isPlayingBgm(const char* playName);
    bool isRunningBgm(const char* playName);
    void stopAllBgm(s32 fadeFrame, bool isForce);
    void tryStopAllBgm(s32 fadeFrame);
    void setTriggerEventForStopAllBgm(const FunctorBase& functor);
    void tryPauseBgmIfNotPlaying(const char* playName, s32 fadeFrame);
    void tryPauseBgmIfLowPriority(const char* playName, s32 fadeFrame);
    void startSituation(const char* situationName, bool isApply, bool isForce, bool isForceStart);
    void startSituation(sead::PtrArray<IBgmParamsChanger>* paramsChangers,
                        const char* situationName, bool isApply, bool isForce, bool isForceStart);
    void endSituation(sead::PtrArray<IBgmParamsChanger>* paramsChangers, const char* situationName,
                      bool isApply, bool isForce);
    void disableLineChange();
    bool isDisableLineChange();
    bool isUsedLineGroupName(const char* groupName) const;
    BgmLine* getPlayingBgmLine(const char* playName) const;
    BgmLine* getBgmLineByLineName(const char* lineName) const;
    BgmLine* getActiveBgmLineWithoutUpperLayer() const;
    s64 getLoopEndSamplePosition(const char* playName) const;
    void setAudioBusSendController(AudioBusSendController* controller);
    BgmMultiPlayingController* tryAllocMultiPlayingController();
    void tryReleaseMultiPlayingController(BgmMultiPlayingController* controller);
    void deactiveAllBgmMultiPlayingController(s32 fadeFrame);
    s32 getBgmLineNum(bool isWithoutUpperLayer);
    BgmLine* getBgmLineAccessor(s32 index, bool isWithoutUpperLayer);
    s32 getBgmLineIndex(const char* lineName, bool isWithoutUpperLayer) const;

    BgmDataBase* getBgmDataBase() const { return mBgmDataBase; }

    BgmLineKeeper* getBgmLineKeeper() const { return mBgmLineKeeper; }

    BgmSituationDirector* getBgmSituationDirector() const { return mBgmSituationDirector; }

    Bgm3DParamsController* getBgm3DParamsController() const { return mBgm3DParamsController; }

    BgmRhythmSyncDirector* getBgmRhythmSyncDirector() const { return mBgmRhythmSyncDirector; }

private:
    BgmDataBase* mBgmDataBase = nullptr;
    BgmDirector* mDependentModule = nullptr;
    BgmLineKeeper* mBgmLineKeeper = nullptr;
    u8 _20[0x28] = {};
    BgmSituationDirector* mBgmSituationDirector = nullptr;
    Bgm3DParamsController* mBgm3DParamsController = nullptr;
    sead::PtrArray<BgmMultiPlayingController>* mMultiPlayingControllers = nullptr;
    BgmRhythmSyncDirector* mBgmRhythmSyncDirector = nullptr;
    AudioBusSendController* mAudioBusSendController = nullptr;
};

static_assert(sizeof(BgmDirector) == 0x70);
}  // namespace al
