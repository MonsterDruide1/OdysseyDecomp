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

void setAllAnimFrame(LiveActor*, f32);
void setAllAnimFrameRate(LiveActor* f32);

// Skl

bool tryStartSklAnimIfExist(LiveActor*, const char*);
bool tryStartSklAnimIfNotPlaying(LiveActor*, const char*);

void startSklAnim(LiveActor*, const char*);
void startSklAnimInterpole(LiveActor*, const char*, const char*);
void startSklAnimBlend(LiveActor*, const char*, const char*, const char*, const char*, const char*,
                       const char*);
void startSklAnimBlendInterpole(LiveActor*, const char*, const char*, const char*, const char*,
                                const char*, const char*, const char*);

void clearSklAnimInterpole(LiveActor*);
void validateSklAnimRetargetting(const LiveActor*);
void invalidateSklAnimRetargetting(const LiveActor*);
void copySklAnim(LiveActor*, const LiveActor*);

void setSklAnimFrame(LiveActor*, f32, s32);
void setSklAnimFrameRate(LiveActor*, f32, s32);
void setSklAnimFrameAndStop(LiveActor*, f32, s32);
void setSklAnimBlendWeight(LiveActor*, f32, s32);
void setSklAnimBlendWeightDouble(LiveActor*, f32);
void setSklAnimBlendWeightDouble(LiveActor*, f32, f32);
void setSklAnimBlendWeightTriple(LiveActor*, f32, f32, f32);
void setSklAnimBlendWeightQuad(LiveActor*, f32, f32, f32, f32);
void setSklAnimBlendWeightFivefold(LiveActor*, f32, f32, f32, f32, f32);
void setSklAnimBlendWeightSixfold(LiveActor*, f32, f32, f32, f32, f32);
void setSklAnimBlendFrameAll(LiveActor*, f32, bool);
void setSklAnimBlendFrameRateAll(LiveActor*, f32, bool);

const char* getPlayingSklAnimName(const LiveActor*, s32);
f32 getSklAnimFrame(const LiveActor*, s32);
f32 getSklAnimFrameRate(const LiveActor*, s32);
f32 getSklAnimFrameMax(const LiveActor*, s32);
f32 getSklAnimFrameMax(const LiveActor*, const char*);
s32 getSklAnimBlendNum(const LiveActor*);
f32 getSklAnimBlendWeight(const LiveActor*, s32);

bool isSklAnimExist(const LiveActor*, const char*);
bool isSklAnimPlaying(const LiveActor*, const char*, s32);
bool isExistSklAnimRetargetting(const LiveActor*);
bool isSklAnimRetargettingValid(const LiveActor*);
bool isSklAnimExist(const LiveActor*);
bool isSklAnimEnd(const LiveActor*, s32);
bool isSklAnimOneTime(const LiveActor*, const char*);
bool isSklAnimOneTime(const LiveActor*, s32);
bool isSklAnimPlaying(const LiveActor*, s32);

// Mtp

bool tryStartMtpAnimIfExist(LiveActor*, const char*);
bool tryStartMtpAnimIfNotPlaying(LiveActor*, const char*);

void startMtpAnim(LiveActor*, const char*);
void startMtpAnimAndSetFrameAndStop(LiveActor*, const char*, f32);

void clearMtpAnim(LiveActor*);

void setMtpAnimFrame(LiveActor*, f32);
void setMtpAnimFrameRate(LiveActor*, f32);
void setMtpAnimFrameAndStop(LiveActor*, f32);
void setMtpAnimFrameAndStopEnd(LiveActor*);

const char* getPlayingMtpAnimName(const LiveActor*);
f32 getMtpAnimFrame(const LiveActor*);
f32 getMtpAnimFrameRate(const LiveActor*);
f32 getMtpAnimFrameMax(const LiveActor*);
f32 getMtpAnimFrameMax(const LiveActor*, const char*);

bool isMtpAnimExist(const LiveActor*, const char*);
bool isMtpAnimPlaying(const LiveActor*, const char*);
bool isMtpAnimExist(const LiveActor*);
bool isMtpAnimEnd(const LiveActor*);
bool isMtpAnimOneTime(const LiveActor*, const char*);
bool isMtpAnimOneTime(const LiveActor*);
bool isMtpAnimPlaying(const LiveActor*);

// Mcl

bool tryStartMclAnimIfExist(LiveActor*, const char*);
bool tryStartMclAnimIfNotPlaying(LiveActor*, const char*);

void startMclAnim(LiveActor*, const char*);
void startMclAnimAndSetFrameAndStop(LiveActor*, const char*, f32);

void clearMclAnim(LiveActor*);

void setMclAnimFrame(LiveActor*, f32);
void setMclAnimFrameRate(LiveActor*, f32);
void setMclAnimFrameAndStop(LiveActor*, f32);
void setMclAnimNormalFrameAndStop(LiveActor*, f32);
void setMclAnimFrameAndStopEnd(LiveActor*);

const char* getPlayingMclAnimName(const LiveActor*);
f32 getMclAnimFrame(const LiveActor*);
f32 getMclAnimFrameRate(const LiveActor*);
f32 getMclAnimFrameMax(const LiveActor*);
f32 getMclAnimFrameMax(const LiveActor*, const char*);

bool isMclAnimExist(const LiveActor*, const char*);
bool isMclAnimPlaying(const LiveActor*, const char*);
bool isMclAnimExist(const LiveActor*);
bool isMclAnimEnd(const LiveActor*);
bool isMclAnimOneTime(const LiveActor*, const char*);
bool isMclAnimOneTime(const LiveActor*);
bool isMclAnimPlaying(const LiveActor*);

// Mts

bool tryStartMtsAnimIfExist(LiveActor*, const char*);
bool tryStartMtsAnimIfNotPlaying(LiveActor*, const char*);

void startMtsAnim(LiveActor*, const char*);
void startMtsAnimAndSetFrameAndStop(LiveActor*, const char*, f32);

void clearMtsAnim(LiveActor*);

void setMtsAnimFrame(LiveActor*, f32);
void setMtsAnimFrameRate(LiveActor*, f32);
void setMtsAnimFrameAndStop(LiveActor*, f32);
void setMtsAnimFrameAndStopEnd(LiveActor*);

const char* getPlayingMtsAnimName(const LiveActor*);
f32 getMtsAnimFrame(const LiveActor*);
f32 getMtsAnimFrameRate(const LiveActor*);
f32 getMtsAnimFrameMax(const LiveActor*);
f32 getMtsAnimFrameMax(const LiveActor*, const char*);

bool isMtsAnimExist(const LiveActor*, const char*);
bool isMtsAnimPlaying(const LiveActor*, const char*);
bool isMtsAnimExist(const LiveActor*);
bool isMtsAnimEnd(const LiveActor*);
bool isMtsAnimOneTime(const LiveActor*, const char*);
bool isMtsAnimOneTime(const LiveActor*);
bool isMtsAnimPlaying(const LiveActor*);

// Mat

bool tryStartMatAnimIfExist(LiveActor*, const char*);
bool tryStartMatAnimIfNotPlaying(LiveActor*, const char*);

void createMatAnimSlotForMcl(LiveActor*);
void createMatAnimSlotForMtp(LiveActor*);
void createMatAnimSlotForMts(LiveActor*);

void startMatAnim(LiveActor, const char*);
void startMatAnimAndSetFrameAndStop(LiveActor, const char*, f32);
void clearMatAnim(LiveActor*);

void setMatAnimFrame(LiveActor*, f32);
void setMatAnimFrameRate(LiveActor*, f32);
void setMatAnimFrameAndStop(LiveActor*, f32);
void setMatAnimFrameAndStopEnd(LiveActor*);

const char* getPlayingMatAnimName(const LiveActor*);
f32 getMatAnimFrame(const LiveActor*);
f32 getMatAnimFrameRate(const LiveActor*);
f32 getMatAnimFrameMax(const LiveActor*);
f32 getMatAnimFrameMax(const LiveActor*, const char*);

bool isMatAnimExist(const LiveActor*, const char*);
bool isMatAnimPlaying(const LiveActor*, const char*);
bool isMatAnimExist(const LiveActor*);
bool isMatAnimEnd(const LiveActor*);
bool isMatAnimOneTime(const LiveActor*, const char*);
bool isMatAnimOneTime(const LiveActor*);
bool isMatAnimPlaying(const LiveActor*);

// Vis

bool tryStartVisAnimIfExist(LiveActor*, const char*);
bool tryStartVisAnimIfNotPlaying(LiveActor*, const char*);

void startVisAnim(LiveActor*, const char*);
void startVisAnimAndSetFrameAndStop(LiveActor*, const char*, f32);

void clearVisAnim(LiveActor*);

void setVisAnimFrame(LiveActor*, f32);
void setVisAnimFrameRate(LiveActor*, f32);
void setVisAnimFrameAndStop(LiveActor*, f32);
void setVisAnimFrameAndStopEnd(LiveActor*);

const char* getPlayingVisAnimName(const LiveActor*);
f32 getVisAnimFrame(const LiveActor*);
f32 getVisAnimFrameRate(const LiveActor*);
f32 getVisAnimFrameMax(const LiveActor*);
f32 getVisAnimFrameMax(const LiveActor*, const char*);

bool isVisAnimExist(const LiveActor*, const char*);
bool isVisAnimPlaying(const LiveActor*, const char*);
bool isVisAnimExist(const LiveActor*);
bool isVisAnimEnd(const LiveActor*);
bool isVisAnimOneTime(const LiveActor*, const char*);
bool isVisAnimOneTime(const LiveActor*);
bool isVisAnimPlaying(const LiveActor*);

// Vis for Action

bool tryStartVisAnimIfExistForAction(LiveActor*, const char*);
bool tryStartVisAnimIfNotPlayingForAction(LiveActor*, const char*);

void startVisAnimForAction(LiveActor*, const char*);
void startVisAnimAndSetFrameAndStopForAction(LiveActor*, const char*, f32);

void clearVisAnimForAction(LiveActor*);

void setVisAnimFrameForAction(LiveActor*, f32);
void setVisAnimFrameRateForAction(LiveActor*, f32);
void setVisAnimFrameAndStopForAction(LiveActor*, f32);
void setVisAnimFrameAndStopEndForAction(LiveActor*);

const char* getPlayingVisAnimNameForAction(const LiveActor*);
f32 getVisAnimFrameForAction(const LiveActor*);
f32 getVisAnimFrameRateForAction(const LiveActor*);
f32 getVisAnimFrameMaxForAction(const LiveActor*);
f32 getVisAnimFrameMaxForAction(const LiveActor*, const char*);

bool isVisAnimExistForAction(const LiveActor*, const char*);
bool isVisAnimPlayingForAction(const LiveActor*, const char*);
bool isVisAnimExistForAction(const LiveActor*);
bool isVisAnimEndForAction(const LiveActor*);
bool isVisAnimOneTimeForAction(const LiveActor*, const char*);
bool isVisAnimOneTimeForAction(const LiveActor*);
bool isVisAnimPlayingForAction(const LiveActor*);

// Skl Retargetting

void initAndBindSklAnimRetargetting(LiveActor*, const char*, const sead::Vector3f&);
SklAnimRetargettingInfo* createSklAnimRetargetting(const LiveActor*, const char*,
                                                   const sead::Vector3f&);
void bindSklAnimRetargetting(const LiveActor*, const SklAnimRetargettingInfo*);
SklAnimRetargettingInfo* createSklAnimRetargetting(const LiveActor*, const LiveActor*,
                                                   const sead::Vector3f&);
void unbindSklAnimRetargetting(const LiveActor);

// Partial Skl

void startPartialSklAnim(LiveActor*, const char*, s32, s32, const SklAnimRetargettingInfo*);
void startPartialSklAnimWithInterpolate(LiveActor*, const char*, s32, s32, s32,
                                        const SklAnimRetargettingInfo*);
void initPartialSklAnim(LiveActor*, s32, s32, s32);
void clearPartialSklAnim(LiveActor*, s32);
void clearPartialSklAnimWithInterpolate(LiveActor*, s32, s32);

void addPartialSklAnimPartsList(LiveActor*, const char*, const char*, s32);
void addPartialSklAnimPartsListRecursive(LiveActor*, const char*, s32);
void setPartialSklAnimFrame(LiveActor*, s32, f32);
void setPartialSklAnimFrameRate(LiveActor*, s32, f32);
void setBaseMtxAndCalcAnim(LiveActor*, const sead::Matrix34f&, const sead::Vector3f&);

const char* getPlayingPartialSklAnimName(const LiveActor*, s32);
f32 getPartialSklAnimFrame(const LiveActor*, s32);
f32 getPartialSklAnimFrameRate(const LiveActor*, s32);
s32 getPartialSklAnimSlotNum(LiveActor*);
s32 calcJoitsAmountFromJoint(LiveActor*, const char*);

bool isPartialSklAnimEnd(const LiveActor*, s32);
bool isPartialSklAnimOneTime(const LiveActor*, s32);
bool isPartialSklAnimAttached(const LiveActor*, s32);
bool isPartialAnimPlaying(const LiveActor*, s32);

}  // namespace al

namespace alAnimFunction {
bool isAllAnimEnd(const al::LiveActor*, s32);
f32 getAllAnimFrame(const al::LiveActor*, s32);
f32 getAllAnimFrameMax(const al::LiveActor*, const char*, s32);
f32 getAllAnimFrameRate(const al::LiveActor*, s32);
const char* getAllAnimName(const al::LiveActor*);
}  // namespace alAnimFunction
