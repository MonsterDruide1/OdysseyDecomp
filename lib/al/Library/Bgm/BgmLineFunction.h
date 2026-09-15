#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <math/seadVector.h>

namespace al {
class AudioInfoListCreateFunctorBase;
template <typename T>
class AudioInfoListWithParts;
class BgmDirector;
class BgmSituationDirector;
class Bgm3DParamsController;
class BgmLine;
class BgmMultiPlayingController;
struct BgmCombinedLineInfo;
struct BgmPlayInfo;
struct BgmResourceInfo;
struct BgmResourceCategoryInfo;
struct BgmStageInfo;
struct BgmSituationInfo;
struct BgmUserInfo;
struct BgmDemoSyncedProcInfo;
struct BgmPlayingRequest;
class FunctorBase;
class IBgmParamsChanger;
class IUseAudioKeeper;
class LiveActor;
class Resource;
class ByamlIter;
struct BgmDemoProcInfo;
struct BgmEnableSituationInfo;
struct BgmResourceSpecificInfo;
struct BgmResourceSuffixControlInfo;
struct BgmResourceSuffixInfo;
struct BgmStartTriggerSituationInfo;

class BgmDataBase {
public:
    static BgmDataBase* create(const char* archiveName, const char* suffix);

    BgmDataBase(Resource* resource, Resource* localizedResource);

    AudioInfoListWithParts<BgmCombinedLineInfo>* getBgmCombinedLineInfoList() const {
        return mBgmCombinedLineInfoList;
    }

    AudioInfoListWithParts<BgmPlayInfo>* getBgmPlayInfoList() const { return mBgmPlayInfoList; }

    AudioInfoListWithParts<BgmResourceInfo>* getBgmResourceInfoList() const {
        return mBgmResourceInfoList;
    }

    AudioInfoListWithParts<BgmResourceCategoryInfo>* getBgmResourceCategoryInfoList() const {
        return mBgmResourceCategoryInfoList;
    }

    AudioInfoListWithParts<BgmStageInfo>* getBgmStageInfoList() const { return mBgmStageInfoList; }

    AudioInfoListWithParts<BgmSituationInfo>* getBgmSituationInfoList() const {
        return mBgmSituationInfoList;
    }

    AudioInfoListWithParts<BgmUserInfo>* getBgmUserInfoList() const { return mBgmUserInfoList; }

    AudioInfoListWithParts<BgmDemoSyncedProcInfo>* getBgmDemoSyncedProcInfoList() const {
        return mBgmDemoSyncedProcInfoList;
    }

private:
    AudioInfoListWithParts<BgmCombinedLineInfo>* mBgmCombinedLineInfoList;
    AudioInfoListWithParts<BgmPlayInfo>* mBgmPlayInfoList;
    AudioInfoListWithParts<BgmResourceInfo>* mBgmResourceInfoList;
    AudioInfoListWithParts<BgmResourceCategoryInfo>* mBgmResourceCategoryInfoList;
    AudioInfoListWithParts<BgmStageInfo>* mBgmStageInfoList;
    AudioInfoListWithParts<BgmSituationInfo>* mBgmSituationInfoList;
    AudioInfoListWithParts<BgmUserInfo>* mBgmUserInfoList;
    AudioInfoListWithParts<BgmDemoSyncedProcInfo>* mBgmDemoSyncedProcInfoList;
};

static_assert(sizeof(BgmDataBase) == 0x40);

struct BgmCombinedLineInfo {
    static BgmCombinedLineInfo* createInfo(const ByamlIter& iter);
    static s32 compareInfo(const BgmCombinedLineInfo* lhs, const BgmCombinedLineInfo* rhs);

    const char* name = nullptr;
};

struct BgmPlayInfo {
    static BgmPlayInfo* createInfo(const ByamlIter& iter);
    static s32 compareInfo(const BgmPlayInfo* lhs, const BgmPlayInfo* rhs);

    const char* name = nullptr;
    const char* lineName = nullptr;
};

struct BgmResourceInfo {
    static BgmResourceInfo* createInfo(const ByamlIter& iter);
    static s32 compareInfo(const BgmResourceInfo* lhs, const BgmResourceInfo* rhs);

    const char* name = nullptr;
    const char* _8 = nullptr;
    const char* categoryName = nullptr;
    BgmResourceSpecificInfo* resourceSpecificInfo = nullptr;
    AudioInfoListWithParts<BgmResourceSuffixInfo>* resourceSuffixInfoList = nullptr;
    AudioInfoListWithParts<BgmEnableSituationInfo>* enableSituationInfoList = nullptr;
    AudioInfoListWithParts<BgmStartTriggerSituationInfo>* startTriggerSituationInfoList = nullptr;
    BgmResourceSuffixControlInfo* resourceSuffixControlInfo = nullptr;
    bool isEnableRegionJump = false;
    bool isDisableAudioEffect = false;
    bool isEnableNwRender = false;
    u8 _43 = 0;
    u32 resourceId = 0;
};

static_assert(sizeof(BgmResourceInfo) == 0x48);

struct BgmResourceCategoryInfo {
    static BgmResourceCategoryInfo* createInfo(const ByamlIter& iter);
    static s32 compareInfo(const BgmResourceCategoryInfo* lhs, const BgmResourceCategoryInfo* rhs);

    const char* name = nullptr;
};

struct BgmStageInfo {
    static BgmStageInfo* createInfo(const ByamlIter& iter);
    static s32 compareInfo(const BgmStageInfo* lhs, const BgmStageInfo* rhs);

    const char* name = nullptr;
};

struct BgmSituationInfo {
    static BgmSituationInfo* createInfo(const ByamlIter& iter);
    static s32 compareInfo(const BgmSituationInfo* lhs, const BgmSituationInfo* rhs);

    const char* name = nullptr;
};

struct BgmDemoSyncedProcInfo {
    static BgmDemoSyncedProcInfo* createInfo(const ByamlIter& iter);
    static s32 compareInfo(const BgmDemoSyncedProcInfo* lhs, const BgmDemoSyncedProcInfo* rhs);

    const char* name = nullptr;
    bool isEnableEvent = false;
    bool isPartsProc = false;
    u8 _a[6] = {};
    sead::PtrArray<const char>* partsProcNameList = nullptr;
    bool isDisableLineChange = false;
    u8 _19[7] = {};
    AudioInfoListWithParts<BgmDemoProcInfo>* demoProcInfoList = nullptr;
    AudioInfoListWithParts<BgmDemoProcInfo>* demoEndProcInfoList = nullptr;
};

static_assert(sizeof(BgmDemoSyncedProcInfo) == 0x30);

BgmDirector* getBgmDirector(const IUseAudioKeeper*);
BgmDirector* tryGetBgmDirector(const IUseAudioKeeper*);
BgmSituationDirector* getBgmSituationDirector(const IUseAudioKeeper*);
Bgm3DParamsController* getBgm3DParamsController(const IUseAudioKeeper*);
BgmLine* getActiveBgmLine(const IUseAudioKeeper*);
BgmLine* getPlayingBgmLine(const IUseAudioKeeper*, const char*);
const char* getCurPlayingBgmPlayName(const IUseAudioKeeper*);
const char* getCurPlayingBgmResourceName(const IUseAudioKeeper*);
const char* getBgmPlayNameInCurPosition(const IUseAudioKeeper*, bool);
const char* getBgmResourceNameInCurPosition(const IUseAudioKeeper*, bool);
s64 getActiveBgmCurSamplePosition(const IUseAudioKeeper*);
s64 getBgmLoopEndSamplePosition(const IUseAudioKeeper*, const char*);
void startBgm(const IUseAudioKeeper*, const char*, s32, s32);
void startBgm(const IUseAudioKeeper*, const BgmPlayingRequest&, bool, bool);
void startBgmWithSuffix(const IUseAudioKeeper*, const char*, const char*, s32, s32);
void startAndStopBgmInCurPosition(const IUseAudioKeeper*, bool);
void startBgmSituation(const IUseAudioKeeper*, const char*, bool);
void prepareBgm(const IUseAudioKeeper*, const BgmPlayingRequest&);
void prepareBgmWithAreaCheck(const IUseAudioKeeper*);
void stopBgm(const IUseAudioKeeper*, const char*, s32);
void stopBgm(const IUseAudioKeeper*, const BgmPlayingRequest&);
void pauseBgm(const IUseAudioKeeper*, const char*, s32);
void resumeBgm(const IUseAudioKeeper*, const char*, s32);
bool isPlayingBgm(const IUseAudioKeeper*);
bool isPlayingBgm(const IUseAudioKeeper*, const char*);
bool isRunningBgm(const IUseAudioKeeper*, const char*);
bool isPlayingResourceCategoryBgm(const IUseAudioKeeper*, const char*);
void stopActiveBgm(const IUseAudioKeeper*, s32);
void pauseActiveBgm(const IUseAudioKeeper*, s32);
void resumeActiveBgm(const IUseAudioKeeper*, s32);
bool isPlayingActiveBgm(const IUseAudioKeeper*, const char*);
bool isPauseActiveBgm(const IUseAudioKeeper*);
void stopAllBgm(const IUseAudioKeeper*, s32);
void tryStopAllBgm(const IUseAudioKeeper*, s32);
void setTriggerEventForStopAllBgm(const IUseAudioKeeper*, const FunctorBase&);
void tryPauseBgmIfDifferBgmArea(const LiveActor*, const sead::Vector3f&, s32);
void tryPauseBgmIfLowPriority(const IUseAudioKeeper*, const char*, s32);
void startBgmSituation(const IUseAudioKeeper*, const char*, bool, bool);
void startBgmSituation(const IUseAudioKeeper*, sead::PtrArray<IBgmParamsChanger>*, const char*,
                       bool, bool);
void forceStartBgmSituation(const IUseAudioKeeper*, const char*, bool, bool);
void endBgmSituation(const IUseAudioKeeper*, const char*, bool);
void endBgmSituation(const IUseAudioKeeper*, sead::PtrArray<IBgmParamsChanger>*, const char*, bool);
void forceEndBgmSituation(const IUseAudioKeeper*, const char*, bool);
void startBgm3D(const IUseAudioKeeper*);
void endBgm3D(const IUseAudioKeeper*);
void changeBgm3DParams(const IUseAudioKeeper*, f32, f32, f32, f32, bool);
void activateBgm3DController(const IUseAudioKeeper*);
void deactivateBgm3DController(const IUseAudioKeeper*);
bool isActivateBgm3DController(const IUseAudioKeeper*);
s32 getActiveBgmSituationNum(const IUseAudioKeeper*);
const char* getActiveBgmSituationName(const IUseAudioKeeper*, s32);
s32 getDeactiveBgmSituationNum(const IUseAudioKeeper*);
const char* getDeactiveBgmSituationName(const IUseAudioKeeper*, s32);
void takeBgmSituationSnapShot(const IUseAudioKeeper*);
void applyBgmSituationSnapShot(const IUseAudioKeeper*);
void applyNoAppliedSituationRecord(const IUseAudioKeeper*);
bool isStartedBgmSituation(const IUseAudioKeeper*, const char*);
void endAllBgmSituation(const IUseAudioKeeper*);
void clearBgmDataForStepOverScene(const IUseAudioKeeper*);
void clearBgmPlayPointRecord(const IUseAudioKeeper*);
void startBgmSyncedCurBgmBeat(const IUseAudioKeeper*, const BgmPlayingRequest&, s32);
void stopBgmSyncedCurBgmBeat(const IUseAudioKeeper*, const BgmPlayingRequest&, s32);
void disableBgmLineChange(const IUseAudioKeeper*);
void enableBgmLineChange(const IUseAudioKeeper*);
bool isDisableBgmLineChange(const IUseAudioKeeper*);
void disableBgmSituationChange(const IUseAudioKeeper*);
void enableBgmSituationChange(const IUseAudioKeeper*, bool);
bool isDisableBgmSituationChange(const IUseAudioKeeper*);
BgmMultiPlayingController* tryAllocBgmMultiPlayingController(const IUseAudioKeeper*);
void tryReleaseBgmMultiPlayingController(const IUseAudioKeeper*, BgmMultiPlayingController*);
s32 getBgmLineNum(const IUseAudioKeeper*, bool);
BgmLine* getBgmLineAccessor(const IUseAudioKeeper*, s32, bool);
s32 getgBgmLineIndex(const IUseAudioKeeper*, const char*, bool);
void replaceBgmPlayInfoResourceName(const IUseAudioKeeper*, const char*, const char*);
bool isEnableRhythmAnim(const IUseAudioKeeper*, const char*);
bool isTriggerBeat(const IUseAudioKeeper*);
bool isTriggerBackBeat(const IUseAudioKeeper*);
bool isTriggerMeasureTop(const IUseAudioKeeper*);
f32 getBpm(const IUseAudioKeeper*);
f32 getBeatRate(const IUseAudioKeeper*);
f32 getCurBeat(const IUseAudioKeeper*);
f32 getCurBeatOnMeasure(const IUseAudioKeeper*);
f32 getLoopStartBeat(const IUseAudioKeeper*);
f32 getLoopEndBeat(const IUseAudioKeeper*);
f32 getFramePerMeasure(const IUseAudioKeeper*);
s32 getBeatPerMeasure(const IUseAudioKeeper*);
bool isChangeActiveBgmResourceName(const IUseAudioKeeper*);
bool tryRegistTargetBgmResourceName(const IUseAudioKeeper*, const char*);
void muteOnRunningLineTrack(IUseAudioKeeper*, u32, bool);
void muteOffRunningLineTrack(IUseAudioKeeper*, u32, bool);

}  // namespace al
