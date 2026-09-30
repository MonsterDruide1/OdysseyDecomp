#pragma once

#include <basis/seadTypes.h>

#include "Library/Bgm/BgmResourceCategoryInfo.h"

namespace al {
struct BgmResourceSpecificInfo;
struct BgmRhythmInfo;
struct BgmSampleDataInfo;
template <typename T>
class AudioInfoListWithParts;

class BgmRhythmDetector {
public:
    BgmRhythmDetector();

    void init(AudioInfoListWithParts<BgmRhythmInfo>* rhythmInfoList,
              AudioInfoListWithParts<BgmBpmInfo>* bpmInfoList,
              AudioInfoListWithParts<BgmTimeSignatureInfo>* timeSignatureInfoList,
              f32 beatStartOffsetTime, BgmResourceSpecificInfo* resourceSpecificInfo,
              s64 samplePosition, const BgmSampleDataInfo& sampleDataInfo);
    void update(s64 samplePosition);
    void calcParams(s64 samplePosition);
    void setRhythmInfoList(AudioInfoListWithParts<BgmRhythmInfo>* rhythmInfoList,
                           AudioInfoListWithParts<BgmBpmInfo>* bpmInfoList,
                           AudioInfoListWithParts<BgmTimeSignatureInfo>* timeSignatureInfoList,
                           f32 beatStartOffsetTime);
    void resetRhythmInfoList();
    void forceUninitialized();

    bool isTriggerBeat() const { return mIsTriggerBeat; }

    bool isTriggerBackBeat() const { return mIsTriggerBackBeat; }

    bool isTriggerMeasureTop() const { return mIsTriggerMeasureTop; }

    f32 getBpm() const { return mCurrentBpmInfo ? mCurrentBpmInfo->bpm : -1.0; }

    f32 getBeatRate() const { return mBeatRate; }

    f32 getCurBeat() const { return mCurBeat; }

    f32 getCurBeatOnMeasure() const { return mCurBeatOnMeasure; }

    f32 getLoopStartBeat() const { return mLoopStartBeat; }

    f32 getLoopEndBeat() const { return mLoopEndBeat; }

    f32 getFramePerMeasure() const { return mFramePerMeasure; }

    s32 getBeatPerMeasure() const { return mBeatPerMeasure; }

private:
    s32 _0 = -1;
    u8 _4 = 0;
    bool mIsTriggerBeat = false;
    u8 _6 = 0;
    bool mIsTriggerBackBeat = false;
    bool mIsTriggerMeasureTop = false;
    u8 _9[0x43] = {};
    f32 mBeatRate = -1.0;
    f32 mCurBeat = -1.0;
    f32 mCurBeatOnMeasure = -1.0;
    u8 _58[4] = {};
    f32 mFramePerMeasure = -1.0;
    s32 mBeatPerMeasure = -1;
    u8 _64[0xc] = {};
    BgmBpmInfo* mCurrentBpmInfo = nullptr;
    u8 _78[4] = {};
    f32 mLoopStartBeat = -1.0;
    f32 mLoopEndBeat = -1.0;
};

static_assert(sizeof(BgmRhythmDetector) == 0x88);

}  // namespace al
