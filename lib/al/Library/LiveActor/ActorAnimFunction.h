#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Anim/AnimPlayerSkl.h"
#include "Library/Execute/ExecuteTableHolderUpdate.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Model/ModelCtrl.h"
#include "Library/Model/ModelKeeper.h"

namespace al {
class AnimPlayerMat;
class LiveActor;
struct SklAnimRetargettingInfo;

// Inlined

inline AnimPlayerSkl* getSkl(const LiveActor* actor) {
    return actor->getModelKeeper()->getAnimSkl();
}

inline AnimPlayerMat* getMtp(const LiveActor* actor) {
    return actor->getModelKeeper()->getAnimMtp();
}

inline AnimPlayerMat* getMcl(const LiveActor* actor) {
    return actor->getModelKeeper()->getAnimMcl();
}

inline AnimPlayerMat* getMts(const LiveActor* actor) {
    return actor->getModelKeeper()->getAnimMts();
}

inline AnimPlayerMat* getMat(const LiveActor* actor) {
    return actor->getModelKeeper()->getAnimMat();
}

inline AnimPlayerVis* getVis(const LiveActor* actor) {
    return actor->getModelKeeper()->getAnimVis();
}

inline AnimPlayerVis* getVisForAction(const LiveActor* actor) {
    return actor->getModelKeeper()->getAnimVisForAction();
}

inline void updateModelDraw(const LiveActor* actor) {
    actor->getModelKeeper()->getModelCtrl()->recreateDisplayList();
}

inline void updateActorSystem(LiveActor* actor) {
    alActorSystemFunction::updateExecutorDraw(actor);
}

inline void setSklBlendWeight(const LiveActor* actor, u32 index, f32 weight) {
    getSkl(actor)->setSklAnimBlendWeight(index, weight);
}

inline void createMat(const LiveActor* actor, s32 programType) {
    actor->getModelKeeper()->createMatAnimForProgram(programType);
}

// All

void setAllAnimFrame(LiveActor* actor, f32 frame);
void setAllAnimFrameRate(LiveActor* f32);

// Skl

bool tryStartSklAnimIfExist(LiveActor* actor, const char* animName);
bool tryStartSklAnimIfNotPlaying(LiveActor* actor, const char* animName);

void startSklAnim(LiveActor* actor, const char* animName);
void startSklAnimInterpole(LiveActor* actor, const char* animName0, const char* animName1);
void startSklAnimBlend(LiveActor* actor, const char* animName0, const char* animName1,
                       const char* animName2, const char* animName3, const char* animName4,
                       const char* animName5);
void startSklAnimBlendInterpole(LiveActor* actor, const char* animName0, const char* animName1,
                                const char* animName2, const char* animName3, const char* animName4,
                                const char* animName5, const char* animName6);

void clearSklAnimInterpole(LiveActor* actor);
void validateSklAnimRetargetting(const LiveActor*);
void invalidateSklAnimRetargetting(const LiveActor*);
void copySklAnim(LiveActor*, const LiveActor*);

void setSklAnimFrame(LiveActor* actor, f32 frame, s32 index);
void setSklAnimFrameRate(LiveActor* actor, f32 frameRate, s32 index);
void setSklAnimFrameAndStop(LiveActor* actor, f32 frame, s32 index);
void setSklAnimBlendWeight(LiveActor* actor, f32 weight, s32 index);
void setSklAnimBlendWeightDouble(LiveActor* actor, f32 weight);
void setSklAnimBlendWeightDouble(LiveActor* actor, f32 weight0, f32 weight1);
void setSklAnimBlendWeightTriple(LiveActor* actor, f32 weight0, f32 weight1, f32 weight2);
void setSklAnimBlendWeightQuad(LiveActor* actor, f32 weight0, f32 weight1, f32 weight2,
                               f32 weight3);
void setSklAnimBlendWeightFivefold(LiveActor* actor, f32 weight0, f32 weight1, f32 weight2,
                                   f32 weight3, f32 weight4);
void setSklAnimBlendWeightSixfold(LiveActor*, f32, f32, f32, f32, f32);
void setSklAnimBlendFrameAll(LiveActor* actor, f32 frame, bool isSync);
void setSklAnimBlendFrameRateAll(LiveActor* actor, f32 frameRate, bool isSync);

const char* getPlayingSklAnimName(const LiveActor* actor, s32 index);
f32 getSklAnimFrame(const LiveActor* actor, s32 index);
f32 getSklAnimFrameRate(const LiveActor* actor, s32 index);
f32 getSklAnimFrameMax(const LiveActor* actor, s32 index);
f32 getSklAnimFrameMax(const LiveActor* actor, const char* animName);
s32 getSklAnimBlendNum(const LiveActor* actor);
f32 getSklAnimBlendWeight(const LiveActor* actor, s32 index);

bool isSklAnimExist(const LiveActor* actor, const char* animName);
bool isSklAnimPlaying(const LiveActor* actor, const char* animName, s32 index);
bool isExistSklAnimRetargetting(const LiveActor*);
bool isSklAnimRetargettingValid(const LiveActor*);
bool isSklAnimExist(const LiveActor* actor);
bool isSklAnimEnd(const LiveActor* actor, s32 index);
bool isSklAnimOneTime(const LiveActor* actor, const char* animName);
bool isSklAnimOneTime(const LiveActor* actor, s32 index);
bool isSklAnimPlaying(const LiveActor* actor, s32 index);

// Mtp

bool tryStartMtpAnimIfExist(LiveActor* actor, const char* animName);
bool tryStartMtpAnimIfNotPlaying(LiveActor* actor, const char* animName);

void startMtpAnim(LiveActor* actor, const char* animName);
void startMtpAnimAndSetFrameAndStop(LiveActor* actor, const char* animName, f32 frame);

void clearMtpAnim(LiveActor* actor);

void setMtpAnimFrame(LiveActor* actor, f32 frame);
void setMtpAnimFrameRate(LiveActor* actor, f32 frameRate);
void setMtpAnimFrameAndStop(LiveActor* actor, f32 frame);
void setMtpAnimFrameAndStopEnd(LiveActor* actor);

const char* getPlayingMtpAnimName(const LiveActor* actor);
f32 getMtpAnimFrame(const LiveActor* actor);
f32 getMtpAnimFrameRate(const LiveActor* actor);
f32 getMtpAnimFrameMax(const LiveActor* actor);
f32 getMtpAnimFrameMax(const LiveActor* actor, const char* animName);

bool isMtpAnimExist(const LiveActor* actor, const char* animName);
bool isMtpAnimPlaying(const LiveActor* actor, const char* animName);
bool isMtpAnimExist(const LiveActor* actor);
bool isMtpAnimEnd(const LiveActor* actor);
bool isMtpAnimOneTime(const LiveActor* actor, const char* animName);
bool isMtpAnimOneTime(const LiveActor* actor);
bool isMtpAnimPlaying(const LiveActor* actor);

// Mcl

bool tryStartMclAnimIfExist(LiveActor* actor, const char* animName);
bool tryStartMclAnimIfNotPlaying(LiveActor* actor, const char* animName);

void startMclAnim(LiveActor* actor, const char* animName);
void startMclAnimAndSetFrameAndStop(LiveActor* actor, const char* animName, f32 frame);

void clearMclAnim(LiveActor* actor);

void setMclAnimFrame(LiveActor* actor, f32 frame);
void setMclAnimFrameRate(LiveActor* actor, f32 frameRate);
void setMclAnimFrameAndStop(LiveActor* actor, f32 frame);
void setMclAnimNormalFrameAndStop(LiveActor* actor, f32 frame);
void setMclAnimFrameAndStopEnd(LiveActor* actor);

const char* getPlayingMclAnimName(const LiveActor* actor);
f32 getMclAnimFrame(const LiveActor* actor);
f32 getMclAnimFrameRate(const LiveActor* actor);
f32 getMclAnimFrameMax(const LiveActor* actor);
f32 getMclAnimFrameMax(const LiveActor* actor, const char* animName);

bool isMclAnimExist(const LiveActor* actor, const char* animName);
bool isMclAnimPlaying(const LiveActor* actor, const char* animName);
bool isMclAnimExist(const LiveActor* actor);
bool isMclAnimEnd(const LiveActor* actor);
bool isMclAnimOneTime(const LiveActor* actor, const char* animName);
bool isMclAnimOneTime(const LiveActor* actor);
bool isMclAnimPlaying(const LiveActor* actor);

// Mts

bool tryStartMtsAnimIfExist(LiveActor* actor, const char* animName);
bool tryStartMtsAnimIfNotPlaying(LiveActor* actor, const char* animName);

void startMtsAnim(LiveActor* actor, const char* animName);
void startMtsAnimAndSetFrameAndStop(LiveActor* actor, const char* animName, f32 frame);

void clearMtsAnim(LiveActor* actor);

void setMtsAnimFrame(LiveActor* actor, f32 frame);
void setMtsAnimFrameRate(LiveActor* actor, f32 frameRate);
void setMtsAnimFrameAndStop(LiveActor* actor, f32 frame);
void setMtsAnimFrameAndStopEnd(LiveActor* actor);

const char* getPlayingMtsAnimName(const LiveActor* actor);
f32 getMtsAnimFrame(const LiveActor* actor);
f32 getMtsAnimFrameRate(const LiveActor* actor);
f32 getMtsAnimFrameMax(const LiveActor* actor);
f32 getMtsAnimFrameMax(const LiveActor* actor, const char* animName);

bool isMtsAnimExist(const LiveActor* actor, const char* animName);
bool isMtsAnimPlaying(const LiveActor* actor, const char* animName);
bool isMtsAnimExist(const LiveActor* actor);
bool isMtsAnimEnd(const LiveActor* actor);
bool isMtsAnimOneTime(const LiveActor* actor, const char* animName);
bool isMtsAnimOneTime(const LiveActor* actor);
bool isMtsAnimPlaying(const LiveActor* actor);

// Mat

bool tryStartMatAnimIfExist(LiveActor* actor, const char* animName);
bool tryStartMatAnimIfNotPlaying(LiveActor* actor, const char* animName);

void createMatAnimSlotForMcl(LiveActor* actor);
void createMatAnimSlotForMtp(LiveActor* actor);
void createMatAnimSlotForMts(LiveActor* actor);

void startMatAnim(LiveActor, const char*);
void startMatAnimAndSetFrameAndStop(LiveActor, const char*, f32);
void clearMatAnim(LiveActor* actor);

void setMatAnimFrame(LiveActor* actor, f32 frame);
void setMatAnimFrameRate(LiveActor* actor, f32 frameRate);
void setMatAnimFrameAndStop(LiveActor* actor, f32 frame);
void setMatAnimFrameAndStopEnd(LiveActor* actor);

const char* getPlayingMatAnimName(const LiveActor* actor);
f32 getMatAnimFrame(const LiveActor* actor);
f32 getMatAnimFrameRate(const LiveActor* actor);
f32 getMatAnimFrameMax(const LiveActor* actor);
f32 getMatAnimFrameMax(const LiveActor* actor, const char* animName);

bool isMatAnimExist(const LiveActor* actor, const char* animName);
bool isMatAnimPlaying(const LiveActor* actor, const char* animName);
bool isMatAnimExist(const LiveActor* actor);
bool isMatAnimEnd(const LiveActor* actor);
bool isMatAnimOneTime(const LiveActor* actor, const char* animName);
bool isMatAnimOneTime(const LiveActor* actor);
bool isMatAnimPlaying(const LiveActor* actor);

// Vis

bool tryStartVisAnimIfExist(LiveActor* actor, const char* animName);
bool tryStartVisAnimIfNotPlaying(LiveActor* actor, const char* animName);

void startVisAnim(LiveActor* actor, const char* animName);
void startVisAnimAndSetFrameAndStop(LiveActor* actor, const char* animName, f32 frame);

void clearVisAnim(LiveActor* actor);

void setVisAnimFrame(LiveActor* actor, f32 frame);
void setVisAnimFrameRate(LiveActor* actor, f32 frameRate);
void setVisAnimFrameAndStop(LiveActor* actor, f32 frame);
void setVisAnimFrameAndStopEnd(LiveActor* actor);

const char* getPlayingVisAnimName(const LiveActor* actor);
f32 getVisAnimFrame(const LiveActor* actor);
f32 getVisAnimFrameRate(const LiveActor* actor);
f32 getVisAnimFrameMax(const LiveActor* actor);
f32 getVisAnimFrameMax(const LiveActor* actor, const char* animName);

bool isVisAnimExist(const LiveActor* actor, const char* animName);
bool isVisAnimPlaying(const LiveActor* actor, const char* animName);
bool isVisAnimExist(const LiveActor* actor);
bool isVisAnimEnd(const LiveActor* actor);
bool isVisAnimOneTime(const LiveActor* actor, const char* animName);
bool isVisAnimOneTime(const LiveActor* actor);
bool isVisAnimPlaying(const LiveActor* actor);

// Vis for Action

bool tryStartVisAnimIfExistForAction(LiveActor* actor, const char* animName);
bool tryStartVisAnimIfNotPlayingForAction(LiveActor* actor, const char* animName);

void startVisAnimForAction(LiveActor* actor, const char* animName);
void startVisAnimAndSetFrameAndStopForAction(LiveActor* actor, const char* animName, f32 frame);

void clearVisAnimForAction(LiveActor* actor);

void setVisAnimFrameForAction(LiveActor* actor, f32 frame);
void setVisAnimFrameRateForAction(LiveActor* actor, f32 frameRate);
void setVisAnimFrameAndStopForAction(LiveActor* actor, f32 frame);
void setVisAnimFrameAndStopEndForAction(LiveActor* actor);

const char* getPlayingVisAnimNameForAction(const LiveActor* actor);
f32 getVisAnimFrameForAction(const LiveActor* actor);
f32 getVisAnimFrameRateForAction(const LiveActor* actor);
f32 getVisAnimFrameMaxForAction(const LiveActor* actor);
f32 getVisAnimFrameMaxForAction(const LiveActor*, const char*);

bool isVisAnimExistForAction(const LiveActor*, const char*);
bool isVisAnimPlayingForAction(const LiveActor* actor, const char* animName);
bool isVisAnimExistForAction(const LiveActor*);
bool isVisAnimEndForAction(const LiveActor* actor);
bool isVisAnimOneTimeForAction(const LiveActor* actor, const char* animName);
bool isVisAnimOneTimeForAction(const LiveActor* actor);
bool isVisAnimPlayingForAction(const LiveActor* actor);

// Skl Retargetting

void initAndBindSklAnimRetargetting(LiveActor*, const char*, const sead::Vector3f&);
SklAnimRetargettingInfo* createSklAnimRetargetting(const LiveActor*, const char*,
                                                   const sead::Vector3f&);
void bindSklAnimRetargetting(const LiveActor*, const SklAnimRetargettingInfo*);
SklAnimRetargettingInfo* createSklAnimRetargetting(const LiveActor*, const LiveActor*,
                                                   const sead::Vector3f&);
void unbindSklAnimRetargetting(const LiveActor*);

// Partial Skl

void startPartialSklAnim(LiveActor* actor, const char* animName, s32 partCount, s32 value,
                         const SklAnimRetargettingInfo* info);
void startPartialSklAnimWithInterpolate(LiveActor* actor, const char* animName, s32 partCount,
                                        s32 value, s32 interpol,
                                        const SklAnimRetargettingInfo* info);
void initPartialSklAnim(LiveActor*, s32, s32, s32);
void clearPartialSklAnim(LiveActor* actor, s32 index);
void clearPartialSklAnimWithInterpolate(LiveActor* actor, s32 partIndex, s32 interpol);

void addPartialSklAnimPartsList(LiveActor*, const char*, const char*, s32);
void addPartialSklAnimPartsListRecursive(LiveActor*, const char*, s32);
void setPartialSklAnimFrame(LiveActor* actor, s32 index, f32 frame);
void setPartialSklAnimFrameRate(LiveActor* actor, s32 index, f32 frameRate);
void setBaseMtxAndCalcAnim(LiveActor* actor, const sead::Matrix34f& matrix,
                           const sead::Vector3f& vector);

const char* getPlayingPartialSklAnimName(const LiveActor* actor, s32 index);
f32 getPartialSklAnimFrame(const LiveActor* actor, s32 index);
f32 getPartialSklAnimFrameRate(const LiveActor* actor, s32 index);
s32 getPartialSklAnimSlotNum(LiveActor*);
s32 calcJoitsAmountFromJoint(LiveActor*, const char*);

bool isPartialSklAnimEnd(const LiveActor* actor, s32 index);
bool isPartialSklAnimOneTime(const LiveActor* actor, s32 index);
bool isPartialSklAnimAttached(const LiveActor* actor, s32 index);
bool isPartialAnimPlaying(const LiveActor*, s32);

}  // namespace al

namespace alAnimFunction {
bool isAllAnimEnd(const al::LiveActor*, s32);
f32 getAllAnimFrame(const al::LiveActor*, s32);
f32 getAllAnimFrameMax(const al::LiveActor*, const char*, s32);
f32 getAllAnimFrameRate(const al::LiveActor*, s32);
const char* getAllAnimName(const al::LiveActor*);
}  // namespace alAnimFunction
