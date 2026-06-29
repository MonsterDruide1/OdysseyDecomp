#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

namespace al {
class AudioBusSendController;
class Bgm;
class BgmDataBase;
class BgmLinePlayInfo;
class BgmParamsChanger;
class BgmPlayPointRecorder;
struct BgmPlayingRequest;
class IBgmParamsChanger;
class SeadAudioPlayer;
struct BgmEnableSituationInfo;
struct BgmLineInfo;
struct BgmMusicalInfo;
struct BgmResourceInfo;
struct BgmResourceSuffixInfo;
struct BgmSampleDataInfo;
struct BgmResourceSpecificInfo;
template <typename T>
class AudioInfoListWithParts;

class BgmLine {
public:
    BgmLine();

    virtual bool isRunning() const;
    virtual bool isPause() const;
    virtual bool isWaitStart() const;
    virtual bool isFadeOut() const;
    virtual bool isPrepared() const;
    virtual bool isPreparedByPlayName(const char* playName) const;
    virtual bool isRunningByPlayName(const char* playName) const;
    virtual bool isUnnecessaryPrepare(const char* playName) const;
    virtual bool isEnableRhythmDetection() const;
    virtual bool isPlaying() const;
    virtual const char* getCurPlayName() const;
    virtual const char* getCurResourceName() const;
    virtual const char* getLineName() const;
    virtual const char* getResourceName(const char* playName) const;
    virtual s32 getLineIndex() const;
    virtual f32 getCurBpm() const;

    void init(const BgmDataBase* dataBase,
              const AudioInfoListWithParts<BgmMusicalInfo>* musicalInfoList,
              SeadAudioPlayer* seadAudioPlayer, const BgmLineInfo* lineInfo, const char* lineName,
              s32 lineIndex, BgmPlayPointRecorder* playPointRecorder,
              sead::PtrArray<BgmParamsChanger>* paramsChangers);
    void stopBgm(s32 fadeFrame);
    void pauseBgm(s32 fadeFrame);
    void resumeBgm(s32 fadeFrame, bool isLineChange);
    void replacePlayInfoResourceName(const char* playName, const char* resourceName);

    Bgm* getActiveBgmPlayer() const { return mBgmPlayers[mActiveBgmPlayerIndex]; }

private:
    u8 _8[0x40] = {};
    Bgm** mBgmPlayers = nullptr;
    u32 mActiveBgmPlayerIndex = 0;
};

}  // namespace al
