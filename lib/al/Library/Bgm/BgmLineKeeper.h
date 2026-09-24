#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

namespace al {
class AudioBusSendController;
class BgmDataBase;
class BgmLine;
class BgmParamsChanger;
class SeadAudioPlayer;
struct BgmCombinedLineInfo;
struct BgmLineInfo;
struct BgmMusicalInfo;
struct BgmPlayingRequest;
class BgmPlayPointRecorder;
class IBgmParamsChanger;
template <typename T>
class AudioInfoListWithParts;

class BgmLineKeeper {
public:
    BgmLineKeeper();

    void init(const BgmDataBase* dataBase,
              const AudioInfoListWithParts<BgmMusicalInfo>* musicalInfoList,
              SeadAudioPlayer* seadAudioPlayer, const char* userName, const char* lineName,
              s32 lineNum, sead::PtrArray<BgmParamsChanger>* paramsChangers);
    void reset(const char* userName, s32 lineNum);
    void update();
    void tryChangeActiveLine();
    void clearPlayData();
    void clearPlayPointRecord();
    void startBgm(const BgmPlayingRequest& request);
    void prepareBgm(const BgmPlayingRequest& request);
    void stopBgm(const char* playName, s32 fadeFrame);
    void pauseBgm(const char* playName, s32 fadeFrame);
    void resumeBgm(const char* playName, s32 fadeFrame);
    bool isPlayingBgm();
    bool isPlayingBgm(const char* playName);
    bool isRunningBgm(const char* playName);
    bool isPauseActiveBgmLine();
    void pauseBgmIfFadeOut(s32 fadeFrame);
    void resumeBgmIfFadeOut(s32 fadeFrame);
    void stopAllBgmLine(s32 fadeFrame, bool isForce);
    void tryStopAllBgmLine(s32 fadeFrame);
    void tryPauseBgmIfNotPlaying(const char* playName, s32 fadeFrame);
    void tryPauseBgmIfLowPriority(const char* playName, s32 fadeFrame);
    void startSituation(const sead::FixedPtrArray<IBgmParamsChanger, 5>& paramsChangers);
    void endSituation(const sead::FixedPtrArray<IBgmParamsChanger, 5>& paramsChangers);
    s32 getBgmLineNum() const;
    BgmLine* getBgmLine(s32 index) const;
    BgmLine* getPlayingBgmLine(const char* playName) const;
    BgmLine* getActiveBgmLine() const;
    BgmLine* getBgmLineByLineName(const char* lineName) const;
    s32 getBgmLineIndex(const char* lineName) const;
    void setAudioBusSendController(AudioBusSendController* controller);

private:
    u8 _0[0x40] = {};
};

static_assert(sizeof(BgmLineKeeper) == 0x40);

}  // namespace al
