#include "Library/LiveActor/ActorAnimFunction.h"

#include <basis/seadTypes.h>
#include <nn/g3d/ResSkeleton.h>

#include "Library/Anim/SklAnimRetargettingInfo.h"
#include "Library/Base/StringUtil.h"
#include "Library/Execute/ExecuteTableHolderUpdate.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Project/Anim/AnimPlayerVisual.h"

namespace al {
void startSklAnim(LiveActor* actor, const char* animName) {
    startSklAnimBlendInterpole(actor, nullptr, animName, nullptr, nullptr, nullptr, nullptr,
                               nullptr);
}

void startSklAnimInterpole(LiveActor* actor, const char* animName0, const char* animName1) {
    startSklAnimBlendInterpole(actor, animName1, animName0, nullptr, nullptr, nullptr, nullptr,
                               nullptr);
}

bool tryStartSklAnimIfExist(LiveActor* actor, const char* animName) {
    if (!isSklAnimExist(actor) || !getSkl(actor)->isSklAnimExist(animName))
        return false;
    startSklAnim(actor, animName);
    return true;
}

bool isSklAnimExist(const LiveActor* actor, const char* animName) {
    return isSklAnimExist(actor) && getSkl(actor)->isSklAnimExist(animName);
}

bool tryStartSklAnimIfNotPlaying(LiveActor* actor, const char* animName) {
    if (isSklAnimPlaying(actor, animName, 0))
        return false;
    startSklAnim(actor, animName);
    return true;
}

bool isSklAnimPlaying(const LiveActor* actor, const char* animName, s32 index) {
    const char* playingName = getPlayingSklAnimName(actor, index);
    return playingName && isEqualString(animName, playingName);
}

void startSklAnimBlend(LiveActor* actor, const char* animName0, const char* animName1,
                       const char* animName2, const char* animName3, const char* animName4,
                       const char* animName5) {
    startSklAnimBlendInterpole(actor, nullptr, animName0, animName1, animName2, animName3,
                               animName4, animName5);
}

void startSklAnimBlendInterpole(LiveActor* actor, const char* animName0, const char* animName1,
                                const char* animName2, const char* animName3, const char* animName4,
                                const char* animName5, const char* animName6) {
    getSkl(actor)->startSklAnim(animName0, animName1, animName2, animName3, animName4, animName5,
                                animName6);
}

void clearSklAnimInterpole(LiveActor* actor) {
    getSkl(actor)->reset();
}

bool isSklAnimExist(const LiveActor* actor) {
    return getSkl(actor) != nullptr;
}

bool isSklAnimEnd(const LiveActor* actor, s32 index) {
    return getSkl(actor)->isSklAnimEnd(index);
}

bool isSklAnimOneTime(const LiveActor* actor, const char* animName) {
    return getSkl(actor)->isSklAnimOneTime(animName);
}

bool isSklAnimOneTime(const LiveActor* actor, s32 index) {
    return getSkl(actor)->isSklAnimOneTime(index);
}

bool isSklAnimPlaying(const LiveActor* actor, s32 index) {
    return getSkl(actor)->isSklAnimPlaying(index);
}

const char* getPlayingSklAnimName(const LiveActor* actor, s32 index) {
    return getSkl(actor)->getPlayingSklAnimName(index);
}

f32 getSklAnimFrame(const LiveActor* actor, s32 index) {
    return getSkl(actor)->getSklAnimFrame(index);
}

f32 getSklAnimFrameRate(const LiveActor* actor, s32 index) {
    return getSkl(actor)->getSklAnimFrameRate(index);
}

f32 getSklAnimFrameMax(const LiveActor* actor, s32 index) {
    return getSkl(actor)->getSklAnimFrameMax(index);
}

f32 getSklAnimFrameMax(const LiveActor* actor, const char* animName) {
    return getSkl(actor)->getSklAnimFrameMax(animName);
}

void setSklAnimFrame(LiveActor* actor, f32 frame, s32 index) {
    getSkl(actor)->setSklAnimFrame(index, frame);
}

void setSklAnimFrameRate(LiveActor* actor, f32 frameRate, s32 index) {
    getSkl(actor)->setSklAnimFrameRate(index, frameRate);
}

void setSklAnimFrameAndStop(LiveActor* actor, f32 frame, s32 index) {
    setSklAnimFrame(actor, frame, index);
    setSklAnimFrameRate(actor, 0.0, index);
}

s32 getSklAnimBlendNum(const LiveActor* actor) {
    return getSkl(actor)->getSklAnimBlendNum();
}

f32 getSklAnimBlendWeight(const LiveActor* actor, s32 index) {
    return getSkl(actor)->getSklAnimBlendWeight(index);
}

void setSklAnimBlendWeight(LiveActor* actor, f32 weight, s32 index) {
    setSklBlendWeight(actor, index, weight);
    getSkl(actor)->calcSklAnim();
}

void setSklAnimBlendWeightDouble(LiveActor* actor, f32 weight) {
    setSklBlendWeight(actor, 0, weight);
    setSklBlendWeight(actor, 1, 1 - weight);
    getSkl(actor)->calcSklAnim();
}

void setSklAnimBlendWeightDouble(LiveActor* actor, f32 weight0, f32 weight1) {
    setSklBlendWeight(actor, 0, weight0);
    setSklBlendWeight(actor, 1, weight1);
    getSkl(actor)->calcSklAnim();
}

void setSklAnimBlendWeightTriple(LiveActor* actor, f32 weight0, f32 weight1, f32 weight2) {
    setSklBlendWeight(actor, 0, weight0);
    setSklBlendWeight(actor, 1, weight1);
    setSklBlendWeight(actor, 2, weight2);
    getSkl(actor)->calcSklAnim();
}

void setSklAnimBlendWeightQuad(LiveActor* actor, f32 weight0, f32 weight1, f32 weight2,
                               f32 weight3) {
    setSklBlendWeight(actor, 0, weight0);
    setSklBlendWeight(actor, 1, weight1);
    setSklBlendWeight(actor, 2, weight2);
    setSklBlendWeight(actor, 3, weight3);
    getSkl(actor)->calcSklAnim();
}

void setSklAnimBlendWeightFivefold(LiveActor* actor, f32 weight0, f32 weight1, f32 weight2,
                                   f32 weight3, f32 weight4) {
    setSklBlendWeight(actor, 0, weight0);
    setSklBlendWeight(actor, 1, weight1);
    setSklBlendWeight(actor, 2, weight2);
    setSklBlendWeight(actor, 3, weight3);
    setSklBlendWeight(actor, 4, weight4);
    getSkl(actor)->calcSklAnim();
}

void setSklAnimBlendWeightSixfold(LiveActor* actor, f32 weight0, f32 weight1, f32 weight2,
                                  f32 weight3, f32 weight4, f32 weight5) {
    setSklBlendWeight(actor, 0, weight0);
    setSklBlendWeight(actor, 1, weight1);
    setSklBlendWeight(actor, 2, weight2);
    setSklBlendWeight(actor, 3, weight3);
    setSklBlendWeight(actor, 4, weight4);
    setSklBlendWeight(actor, 5, weight5);
    getSkl(actor)->calcSklAnim();
}

void setSklAnimBlendFrameAll(LiveActor* actor, f32 frame, bool isSync) {
    AnimPlayerSkl* sklAnim = getSkl(actor);

    sklAnim->setSklAnimFrame(0, frame);
    if (isSync) {
        f32 frameMax0 = sklAnim->getSklAnimFrameMax(0);
        for (s32 i = 1; i < sklAnim->getSklAnimBlendNum(); i++) {
            if (getSkl(actor)->isSklAnimPlaying(i)) {
                f32 frameCalc = (frame / frameMax0) * sklAnim->getSklAnimFrameMax(i);
                getSkl(actor)->setSklAnimFrame(i, frameCalc);
            }
        }
    } else {
        for (s32 j = 1; j < sklAnim->getSklAnimBlendNum(); j++)
            if (getSkl(actor)->isSklAnimPlaying(j))
                getSkl(actor)->setSklAnimFrame(j, frame);
    }
}

void setSklAnimBlendFrameRateAll(LiveActor* actor, f32 frameRate, bool isSync) {
    AnimPlayerSkl* sklAnim = getSkl(actor);

    sklAnim->setSklAnimFrameRate(0, frameRate);
    if (isSync) {
        f32 frameMax0 = sklAnim->getSklAnimFrameMax(0);
        for (s32 i = 1; i < sklAnim->getSklAnimBlendNum(); i++) {
            if (getSkl(actor)->isSklAnimPlaying(i)) {
                f32 frameCalc = (frameRate / frameMax0) * sklAnim->getSklAnimFrameMax(i);
                getSkl(actor)->setSklAnimFrameRate(i, frameCalc);
            }
        }
    } else {
        for (s32 j = 1; j < sklAnim->getSklAnimBlendNum(); j++)
            if (getSkl(actor)->isSklAnimPlaying(j))
                getSkl(actor)->setSklAnimFrameRate(j, frameRate);
    }
}

void startMtpAnim(LiveActor* actor, const char* animName) {
    getMtp(actor)->startAnim(animName);
    updateModelDraw(actor);
}

void startMtpAnimAndSetFrameAndStop(LiveActor* actor, const char* animName, f32 frame) {
    startMtpAnim(actor, animName);
    setMtpAnimFrame(actor, frame);
    setMtpAnimFrameRate(actor, 0.0);
}

void setMtpAnimFrameRate(LiveActor* actor, f32 frameRate) {
    getMtp(actor)->setAnimFrameRate(frameRate);
}

void setMtpAnimFrame(LiveActor* actor, f32 frame) {
    getMtp(actor)->setAnimFrame(frame);
    updateModelDraw(actor);
}

bool tryStartMtpAnimIfExist(LiveActor* actor, const char* animName) {
    if (!isMtpAnimExist(actor) || !getMtp(actor)->isAnimExist(animName))
        return false;
    startMtpAnim(actor, animName);
    return true;
}

bool isMtpAnimExist(const LiveActor* actor, const char* animName) {
    return getMtp(actor) && getMtp(actor)->isAnimExist(animName);
}

bool tryStartMtpAnimIfNotPlaying(LiveActor* actor, const char* animName) {
    if (isMtpAnimPlaying(actor, animName))
        return false;
    startMtpAnim(actor, animName);
    return true;
}

bool isMtpAnimPlaying(const LiveActor* actor, const char* animName) {
    return isEqualString(animName, getPlayingMtpAnimName(actor));
}

void clearMtpAnim(LiveActor* actor) {
    getMtp(actor)->clearAnim();
    updateModelDraw(actor);
}

bool isMtpAnimExist(const LiveActor* actor) {
    return getMtp(actor) != nullptr;
}

bool isMtpAnimEnd(const LiveActor* actor) {
    return getMtp(actor)->isAnimEnd();
}

bool isMtpAnimOneTime(const LiveActor* actor, const char* animName) {
    return getMtp(actor)->isAnimOneTime(animName);
}

bool isMtpAnimOneTime(const LiveActor* actor) {
    return getMtp(actor)->isAnimOneTime();
}

const char* getPlayingMtpAnimName(const LiveActor* actor) {
    return getMtp(actor)->getPlayingAnimName();
}

bool isMtpAnimPlaying(const LiveActor* actor) {
    return isMtpAnimExist(actor) && getMtp(actor)->isAnimPlaying();
}

f32 getMtpAnimFrame(const LiveActor* actor) {
    return getMtp(actor)->getAnimFrame();
}

f32 getMtpAnimFrameRate(const LiveActor* actor) {
    return getMtp(actor)->getAnimFrameRate();
}

f32 getMtpAnimFrameMax(const LiveActor* actor) {
    return getMtp(actor)->getAnimFrameMax();
}

f32 getMtpAnimFrameMax(const LiveActor* actor, const char* animName) {
    return getMtp(actor)->getAnimFrameMax(animName);
}

void setMtpAnimFrameAndStop(LiveActor* actor, f32 frame) {
    setMtpAnimFrame(actor, frame);
    setMtpAnimFrameRate(actor, 0.0);
}

void setMtpAnimFrameAndStopEnd(LiveActor* actor) {
    setMtpAnimFrameAndStop(actor, getMtpAnimFrameMax(actor));
}

void startMclAnim(LiveActor* actor, const char* animName) {
    getMcl(actor)->startAnim(animName);
}

bool isMclAnimExist(const LiveActor* actor) {
    return getMcl(actor) != nullptr;
}

bool isMclAnimExist(const LiveActor* actor, const char* animName) {
    return isMclAnimExist(actor) && getMcl(actor)->isAnimExist(animName);
}

bool tryStartMclAnimIfExist(LiveActor* actor, const char* animName) {
    if (!isMclAnimExist(actor) || !getMcl(actor)->isAnimExist(animName))
        return false;
    startMclAnim(actor, animName);
    return true;
}

bool tryStartMclAnimIfNotPlaying(LiveActor* actor, const char* animName) {
    if (isMclAnimPlaying(actor, animName))
        return false;
    startMclAnim(actor, animName);
    return true;
}

bool isMclAnimPlaying(const LiveActor* actor, const char* animName) {
    return isEqualString(animName, getPlayingMclAnimName(actor));
}

void clearMclAnim(LiveActor* actor) {
    getMcl(actor)->clearAnim();
}

bool isMclAnimEnd(const LiveActor* actor) {
    return getMcl(actor)->isAnimEnd();
}

bool isMclAnimOneTime(const LiveActor* actor, const char* animName) {
    return getMcl(actor)->isAnimOneTime(animName);
}

bool isMclAnimOneTime(const LiveActor* actor) {
    return getMcl(actor)->isAnimOneTime();
}

const char* getPlayingMclAnimName(const LiveActor* actor) {
    return getMcl(actor)->getPlayingAnimName();
}

bool isMclAnimPlaying(const LiveActor* actor) {
    return isMclAnimExist(actor) && getMcl(actor)->isAnimPlaying();
}

f32 getMclAnimFrame(const LiveActor* actor) {
    return getMcl(actor)->getAnimFrame();
}

f32 getMclAnimFrameRate(const LiveActor* actor) {
    return getMcl(actor)->getAnimFrameRate();
}

f32 getMclAnimFrameMax(const LiveActor* actor) {
    return getMcl(actor)->getAnimFrameMax();
}

f32 getMclAnimFrameMax(const LiveActor* actor, const char* animName) {
    return getMcl(actor)->getAnimFrameMax(animName);
}

void setMclAnimFrame(LiveActor* actor, f32 frame) {
    getMcl(actor)->setAnimFrame(frame);
}

void setMclAnimFrameRate(LiveActor* actor, f32 frameRate) {
    getMcl(actor)->setAnimFrameRate(frameRate);
}

void startMclAnimAndSetFrameAndStop(LiveActor* actor, const char* animName, f32 frame) {
    startMclAnim(actor, animName);
    setMclAnimFrame(actor, frame);
    setMclAnimFrameRate(actor, 0.0);
}

void setMclAnimFrameAndStop(LiveActor* actor, f32 frame) {
    setMclAnimFrame(actor, frame);
    setMclAnimFrameRate(actor, 0.0);
}

void setMclAnimNormalFrameAndStop(LiveActor* actor, f32 frame) {
    setMclAnimFrame(actor, frame * getMclAnimFrameMax(actor));
    setMclAnimFrameRate(actor, 0.0);
}

void setMclAnimFrameAndStopEnd(LiveActor* actor) {
    setMclAnimFrame(actor, getMclAnimFrameMax(actor));
    setMclAnimFrameRate(actor, 0.0);
}

void startMtsAnim(LiveActor* actor, const char* animName) {
    getMts(actor)->startAnim(animName);
}

void startMtsAnimAndSetFrameAndStop(LiveActor* actor, const char* animName, f32 frame) {
    startMtsAnim(actor, animName);
    setMtsAnimFrame(actor, frame);
    setMtsAnimFrameRate(actor, 0.0);
}

void setMtsAnimFrame(LiveActor* actor, f32 frame) {
    getMts(actor)->setAnimFrame(frame);
}

void setMtsAnimFrameRate(LiveActor* actor, f32 frameRate) {
    getMts(actor)->setAnimFrameRate(frameRate);
}

bool tryStartMtsAnimIfExist(LiveActor* actor, const char* animName) {
    if (!isMtsAnimExist(actor) || !getMts(actor)->isAnimExist(animName))
        return false;
    startMtsAnim(actor, animName);
    return true;
}

bool isMtsAnimExist(const LiveActor* actor, const char* animName) {
    return isMtsAnimExist(actor) && getMts(actor)->isAnimExist(animName);
}

bool tryStartMtsAnimIfNotPlaying(LiveActor* actor, const char* animName) {
    if (isMtsAnimPlaying(actor, animName))
        return false;
    startMtsAnim(actor, animName);
    return true;
}

bool isMtsAnimPlaying(const LiveActor* actor, const char* animName) {
    return isEqualString(animName, getPlayingMtsAnimName(actor));
}

void clearMtsAnim(LiveActor* actor) {
    getMts(actor)->clearAnim();
}

bool isMtsAnimExist(const LiveActor* actor) {
    return getMts(actor) != nullptr;
}

bool isMtsAnimEnd(const LiveActor* actor) {
    return getMts(actor)->isAnimEnd();
}

bool isMtsAnimOneTime(const LiveActor* actor, const char* animName) {
    return getMts(actor)->isAnimOneTime(animName);
}

bool isMtsAnimOneTime(const LiveActor* actor) {
    return getMts(actor)->isAnimOneTime();
}

const char* getPlayingMtsAnimName(const LiveActor* actor) {
    return getMts(actor)->getPlayingAnimName();
}

bool isMtsAnimPlaying(const LiveActor* actor) {
    return isMtsAnimExist(actor) && getMts(actor)->isAnimPlaying();
}

f32 getMtsAnimFrame(const LiveActor* actor) {
    return getMts(actor)->getAnimFrame();
}

f32 getMtsAnimFrameRate(const LiveActor* actor) {
    return getMts(actor)->getAnimFrameRate();
}

f32 getMtsAnimFrameMax(const LiveActor* actor) {
    return getMts(actor)->getAnimFrameMax();
}

f32 getMtsAnimFrameMax(const LiveActor* actor, const char* animName) {
    return getMts(actor)->getAnimFrameMax(animName);
}

void setMtsAnimFrameAndStop(LiveActor* actor, f32 frame) {
    setMtsAnimFrame(actor, frame);
    setMtsAnimFrameRate(actor, 0.0);
}

void setMtsAnimFrameAndStopEnd(LiveActor* actor) {
    setMtsAnimFrame(actor, getMtsAnimFrameMax(actor));
    setMtsAnimFrameRate(actor, 0.0);
}

void createMatAnimSlotForMcl(LiveActor* actor) {
    createMat(actor, 0);
}

void createMatAnimSlotForMtp(LiveActor* actor) {
    createMat(actor, 1);
}

void createMatAnimSlotForMts(LiveActor* actor) {
    createMat(actor, 2);
}

void startMatAnim(LiveActor* actor, const char* animName) {
    getMat(actor)->startAnim(animName);
    updateModelDraw(actor);
}

void startMatAnimAndSetFrameAndStop(LiveActor* actor, const char* animName, f32 frame) {
    startMatAnim(actor, animName);
    setMatAnimFrame(actor, frame);
    setMatAnimFrameRate(actor, 0.0);
}

void setMatAnimFrame(LiveActor* actor, f32 frame) {
    getMat(actor)->setAnimFrame(frame);
}

void setMatAnimFrameRate(LiveActor* actor, f32 frameRate) {
    getMat(actor)->setAnimFrameRate(frameRate);
}

bool tryStartMatAnimIfExist(LiveActor* actor, const char* animName) {
    if (!isMatAnimExist(actor) || !getMat(actor)->isAnimExist(animName))
        return false;
    startMatAnim(actor, animName);
    return true;
}

bool isMatAnimExist(const LiveActor* actor, const char* animName) {
    return isMatAnimExist(actor) && getMat(actor)->isAnimExist(animName);
}

bool tryStartMatAnimIfNotPlaying(LiveActor* actor, const char* animName) {
    if (isMatAnimPlaying(actor, animName))
        return false;
    startMatAnim(actor, animName);
    return true;
}

bool isMatAnimPlaying(const LiveActor* actor, const char* animName) {
    return isEqualString(animName, getPlayingMatAnimName(actor));
}

void clearMatAnim(LiveActor* actor) {
    getMat(actor)->clearAnim();
    updateModelDraw(actor);
}

bool isMatAnimExist(const LiveActor* actor) {
    return getMat(actor) != nullptr;
}

bool isMatAnimEnd(const LiveActor* actor) {
    return getMat(actor)->isAnimEnd();
}

bool isMatAnimOneTime(const LiveActor* actor, const char* animName) {
    return getMat(actor)->isAnimOneTime(animName);
}

bool isMatAnimOneTime(const LiveActor* actor) {
    return getMat(actor)->isAnimOneTime();
}

const char* getPlayingMatAnimName(const LiveActor* actor) {
    return getMat(actor)->getPlayingAnimName();
}

bool isMatAnimPlaying(const LiveActor* actor) {
    return isMatAnimExist(actor) && getMat(actor)->isAnimPlaying();
}

f32 getMatAnimFrame(const LiveActor* actor) {
    return getMat(actor)->getAnimFrame();
}

f32 getMatAnimFrameRate(const LiveActor* actor) {
    return getMat(actor)->getAnimFrameRate();
}

f32 getMatAnimFrameMax(const LiveActor* actor) {
    return getMat(actor)->getAnimFrameMax();
}

f32 getMatAnimFrameMax(const LiveActor* actor, const char* animName) {
    return getMat(actor)->getAnimFrameMax(animName);
}

void setMatAnimFrameAndStop(LiveActor* actor, f32 frame) {
    setMatAnimFrame(actor, frame);
    setMatAnimFrameRate(actor, 0.0);
}

void setMatAnimFrameAndStopEnd(LiveActor* actor) {
    setMatAnimFrame(actor, getMatAnimFrameMax(actor));
    setMatAnimFrameRate(actor, 0.0);
}

void startVisAnim(LiveActor* actor, const char* animName) {
    getVis(actor)->startAnim(animName);
    updateActorSystem(actor);
}

bool tryStartVisAnimIfExist(LiveActor* actor, const char* animName) {
    if (!isVisAnimExist(actor) || !getVis(actor)->isAnimExist(animName))
        return false;
    startVisAnim(actor, animName);
    return true;
}

bool isVisAnimExist(const LiveActor* actor, const char* animName) {
    return getVis(actor) && getVis(actor)->isAnimExist(animName);
}

bool tryStartVisAnimIfNotPlaying(LiveActor* actor, const char* animName) {
    if (isVisAnimPlaying(actor, animName))
        return false;
    startVisAnim(actor, animName);
    return true;
}

bool isVisAnimPlaying(const LiveActor* actor, const char* animName) {
    return isEqualString(animName, getPlayingVisAnimName(actor));
}

void clearVisAnim(LiveActor* actor) {
    getVis(actor)->clearAnim();
    alActorSystemFunction::updateExecutorDraw(actor);
}

bool isVisAnimExist(const LiveActor* actor) {
    return getVis(actor) != nullptr;
}

bool isVisAnimEnd(const LiveActor* actor) {
    return getVis(actor)->isAnimEnd();
}

bool isVisAnimOneTime(const LiveActor* actor, const char* animName) {
    return getVis(actor)->isAnimOneTime(animName);
}

bool isVisAnimOneTime(const LiveActor* actor) {
    return getVis(actor)->isAnimOneTime();
}

const char* getPlayingVisAnimName(const LiveActor* actor) {
    return getVis(actor)->getPlayingAnimName();
}

bool isVisAnimPlaying(const LiveActor* actor) {
    return isVisAnimExist(actor) && getVis(actor)->isAnimPlaying();
}

f32 getVisAnimFrame(const LiveActor* actor) {
    return getVis(actor)->getAnimFrame();
}

f32 getVisAnimFrameRate(const LiveActor* actor) {
    return getVis(actor)->getAnimFrameRate();
}

f32 getVisAnimFrameMax(const LiveActor* actor) {
    return getVis(actor)->getAnimFrameMax();
}

f32 getVisAnimFrameMax(const LiveActor* actor, const char* animName) {
    return getVis(actor)->getAnimFrameMax(animName);
}

void setVisAnimFrame(LiveActor* actor, f32 frame) {
    getVis(actor)->setAnimFrame(frame);
    updateActorSystem(actor);
}

void setVisAnimFrameRate(LiveActor* actor, f32 frameRate) {
    getVis(actor)->setAnimFrameRate(frameRate);
}

void setVisAnimFrameAndStop(LiveActor* actor, f32 frame) {
    setVisAnimFrame(actor, frame);
    setVisAnimFrameRate(actor, 0.0);
}

void setVisAnimFrameAndStopEnd(LiveActor* actor) {
    setVisAnimFrame(actor, getVisAnimFrameMax(actor));
    setVisAnimFrameRate(actor, 0.0);
}

void startVisAnimAndSetFrameAndStop(LiveActor* actor, const char* animName, f32 frame) {
    startVisAnim(actor, animName);
    setVisAnimFrame(actor, frame);
    setVisAnimFrameRate(actor, 0.0);
}

void startVisAnimForAction(LiveActor* actor, const char* animName) {
    getVisForAction(actor)->startAnim(animName);
    updateActorSystem(actor);
}

bool tryStartVisAnimIfExistForAction(LiveActor* actor, const char* animName) {
    if (!isVisAnimExist(actor) || !getVis(actor)->isAnimExist(animName))
        return false;
    startVisAnimForAction(actor, animName);
    return true;
}

bool tryStartVisAnimIfNotPlayingForAction(LiveActor* actor, const char* animName) {
    if (isVisAnimPlayingForAction(actor, animName))
        return false;
    startVisAnimForAction(actor, animName);
    return true;
}

bool isVisAnimPlayingForAction(const LiveActor* actor, const char* animName) {
    return isEqualString(animName, getPlayingVisAnimNameForAction(actor));
}

void clearVisAnimForAction(LiveActor* actor) {
    getVisForAction(actor)->clearAnim();
    alActorSystemFunction::updateExecutorDraw(actor);
}

bool isVisAnimEndForAction(const LiveActor* actor) {
    return getVisForAction(actor)->isAnimEnd();
}

bool isVisAnimOneTimeForAction(const LiveActor* actor, const char* animName) {
    return getVisForAction(actor)->isAnimOneTime(animName);
}

bool isVisAnimOneTimeForAction(const LiveActor* actor) {
    return getVisForAction(actor)->isAnimOneTime();
}

const char* getPlayingVisAnimNameForAction(const LiveActor* actor) {
    return getVisForAction(actor)->getPlayingAnimName();
}

bool isVisAnimPlayingForAction(const LiveActor* actor) {
    return getVisForAction(actor) && getVisForAction(actor)->isAnimPlaying();
}

f32 getVisAnimFrameForAction(const LiveActor* actor) {
    return getVisForAction(actor)->getAnimFrame();
}

f32 getVisAnimFrameRateForAction(const LiveActor* actor) {
    return getVisForAction(actor)->getAnimFrameRate();
}

f32 getVisAnimFrameMaxForAction(const LiveActor* actor) {
    return getVisForAction(actor)->getAnimFrameMax();
}

void setVisAnimFrameForAction(LiveActor* actor, f32 frame) {
    getVisForAction(actor)->setAnimFrame(frame);
    updateActorSystem(actor);
}

void setVisAnimFrameRateForAction(LiveActor* actor, f32 frameRate) {
    getVisForAction(actor)->setAnimFrameRate(frameRate);
}

void setVisAnimFrameAndStopForAction(LiveActor* actor, f32 frame) {
    setVisAnimFrameForAction(actor, frame);
    setVisAnimFrameRateForAction(actor, 0.0);
}

void setVisAnimFrameAndStopEndForAction(LiveActor* actor) {
    setVisAnimFrameForAction(actor, getVisAnimFrameMaxForAction(actor));
    setVisAnimFrameRateForAction(actor, 0.0);
}

void startVisAnimAndSetFrameAndStopForAction(LiveActor* actor, const char* animName, f32 frame) {
    startVisAnimForAction(actor, animName);
    setVisAnimFrameForAction(actor, frame);
    setVisAnimFrameRateForAction(actor, 0.0);
}

void setAllAnimFrame(LiveActor* actor, f32 frame) {
    if (isSklAnimExist(actor))
        setSklAnimFrame(actor, frame, 0);
    if (isMtpAnimExist(actor))
        setMtpAnimFrame(actor, frame);
    if (isMclAnimExist(actor))
        setMclAnimFrame(actor, frame);
    if (isMtsAnimExist(actor))
        setMtsAnimFrame(actor, frame);
    if (isVisAnimExist(actor))
        setVisAnimFrame(actor, frame);
}

void setAllAnimFrameRate(LiveActor* actor, f32 frameRate) {
    if (isSklAnimExist(actor))
        setSklAnimFrameRate(actor, frameRate, 0);
    if (isMtpAnimExist(actor))
        setMtpAnimFrameRate(actor, frameRate);
    if (isMclAnimExist(actor))
        setMclAnimFrameRate(actor, frameRate);
    if (isMtsAnimExist(actor))
        setMtsAnimFrameRate(actor, frameRate);
    if (isVisAnimExist(actor))
        setVisAnimFrameRate(actor, frameRate);
}

void startPartialSklAnim(LiveActor* actor, const char* animName, s32 partCount, s32 value,
                         const SklAnimRetargettingInfo* info) {
    getSkl(actor)->startPartialAnim(animName, partCount, value, info);
}

void startPartialSklAnimWithInterpolate(LiveActor* actor, const char* animName, s32 partCount,
                                        s32 value, s32 interpol,
                                        const SklAnimRetargettingInfo* info) {
    AnimPlayerSkl* sklAnim = getSkl(actor);
    startPartialSklAnim(actor, animName, partCount, value, info);
    sklAnim->prepareAnimInterpDirect(interpol);
}

void clearPartialSklAnim(LiveActor* actor, s32 index) {
    getSkl(actor)->clearPartialAnim(index);
}

void clearPartialSklAnimWithInterpolate(LiveActor* actor, s32 partIndex, s32 interpol) {
    AnimPlayerSkl* sklAnim = getSkl(actor);
    clearPartialSklAnim(actor, partIndex);
    sklAnim->prepareAnimInterpDirect(interpol);
}

bool isPartialSklAnimEnd(const LiveActor* actor, s32 index) {
    return getSkl(actor)->isPartialAnimEnd(index);
}

bool isPartialSklAnimOneTime(const LiveActor* actor, s32 index) {
    return getSkl(actor)->isPartialAnimOneTime(index);
}

bool isPartialSklAnimAttached(const LiveActor* actor, s32 index) {
    return getSkl(actor)->isPartialAnimAttached(index);
}

bool isPartialSklAnimPlaying(const LiveActor* actor, const char* partsName, s32 index) {
    return isPartialSklAnimAttached(actor, index) &&
           isEqualString(partsName, getPlayingPartialSklAnimName(actor, index));
}

const char* getPlayingPartialSklAnimName(const LiveActor* actor, s32 index) {
    return getSkl(actor)->getPlayingPartialSklAnimName(index);
}

f32 getPartialSklAnimFrame(const LiveActor* actor, s32 index) {
    return getSkl(actor)->getPartialAnimFrame(index);
}

void setPartialSklAnimFrame(LiveActor* actor, s32 index, f32 frame) {
    getSkl(actor)->setPartialAnimFrame(index, frame);
}

f32 getPartialSklAnimFrameRate(const LiveActor* actor, s32 index) {
    return getSkl(actor)->getPartialAnimFrameRate(index);
}

void setPartialSklAnimFrameRate(LiveActor* actor, s32 index, f32 frameRate) {
    getSkl(actor)->setPartialAnimFrameRate(index, frameRate);
}

void setBaseMtxAndCalcAnim(LiveActor* actor, const sead::Matrix34f& matrix,
                           const sead::Vector3f& vector) {
    return actor->getModelKeeper()->calc(matrix, vector);
}
}  // namespace al
