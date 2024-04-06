#include "Library/LiveActor/ActorAnimFunction.h"

#include <basis/seadTypes.h>
#include <nn/g3d/ResSkeleton.h>

#include "Library/Anim/SklAnimRetargettingInfo.h"
#include "Library/Base/String.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/LiveActor.h"
#include "Project/Anim/AnimPlayerVisual.h"

namespace al {
void startSklAnim(LiveActor* actor, const char* animName) {
    getSkl(actor)->startSklAnim(nullptr, animName, nullptr, nullptr, nullptr, nullptr, nullptr);
}
void startSklAnimInterpole(LiveActor* actor, const char* animName0, const char* animName1) {
    getSkl(actor)->startSklAnim(animName1, animName0, nullptr, nullptr, nullptr, nullptr, nullptr);
}
bool tryStartSklAnimIfExist(LiveActor* actor, const char* animName) {
    AnimPlayerSkl* sklAnim = getSkl(actor);

    if (!sklAnim || !sklAnim->isSklAnimExist(animName))
        return false;

    getSkl(actor)->startSklAnim(nullptr, animName, nullptr, nullptr, nullptr, nullptr, nullptr);
    return true;
}
bool isSklAnimExist(const LiveActor* actor, const char* animName) {
    if (!getSkl(actor))
        return false;

    return getSkl(actor)->isSklAnimExist(animName);
}
bool tryStartSklAnimIfNotPlaying(LiveActor* actor, const char* animName) {
    const char* playingName = getSkl(actor)->getPlayingSklAnimName(0);

    if (playingName && isEqualString(animName, playingName))
        return false;
    getSkl(actor)->startSklAnim(nullptr, animName, nullptr, nullptr, nullptr, nullptr, nullptr);
    return true;
}
bool isSklAnimPlaying(const LiveActor* actor, const char* animName, s32 index) {
    const char* playingName = getSkl(actor)->getPlayingSklAnimName(index);
    return playingName && isEqualString(animName, playingName);
}
void startSklAnimBlend(LiveActor* actor, const char* animName0, const char* animName1,
                       const char* animName2, const char* animName3, const char* animName4,
                       const char* animName5) {
    getSkl(actor)->startSklAnim(nullptr, animName0, animName1, animName2, animName3, animName4,
                                animName5);
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
void setSklAnimFrame(LiveActor* actor, f32 frameRate, s32 index) {
    getSkl(actor)->setSklAnimFrame(index, frameRate);
}
void setSklAnimFrameRate(LiveActor* actor, f32 frame, s32 index) {
    getSkl(actor)->setSklAnimFrameRate(index, frame);
}
void setSklAnimFrameAndStop(LiveActor* actor, f32 frame, s32 index) {
    getSkl(actor)->setSklAnimFrame(index, frame);
    getSkl(actor)->setSklAnimFrameRate(index, 0.0);
}
s32 getSklAnimBlendNum(const LiveActor* actor) {
    return getSkl(actor)->getSklAnimBlendNum();
}
f32 getSklAnimBlendWeight(const LiveActor* actor, int index) {
    return getSkl(actor)->getSklAnimBlendWeight(index);
}
void setSklAnimBlendWeight(LiveActor* actor, f32 weight, s32 index) {
    getSkl(actor)->setSklAnimBlendWeight(index, weight);
    getSkl(actor)->calcSklAnim();
}
void setSklAnimBlendWeightDouble(LiveActor* actor, f32 weight) {
    getSkl(actor)->setSklAnimBlendWeight(0, weight);
    f32 weightCalc = 1 - weight;
    getSkl(actor)->setSklAnimBlendWeight(1, weightCalc);
    getSkl(actor)->calcSklAnim();
}
void setSklAnimBlendWeightDouble(LiveActor* actor, f32 weight0, f32 weight1) {
    getSkl(actor)->setSklAnimBlendWeight(0, weight0);
    getSkl(actor)->setSklAnimBlendWeight(1, weight1);
    getSkl(actor)->calcSklAnim();
}
void setSklAnimBlendWeightTriple(LiveActor* actor, f32 weight0, f32 weight1, f32 weight2) {
    getSkl(actor)->setSklAnimBlendWeight(0, weight0);
    getSkl(actor)->setSklAnimBlendWeight(1, weight1);
    getSkl(actor)->setSklAnimBlendWeight(2, weight2);
    getSkl(actor)->calcSklAnim();
}
void setSklAnimBlendWeightQuad(LiveActor* actor, f32 weight0, f32 weight1, f32 weight2,
                               f32 weight3) {
    getSkl(actor)->setSklAnimBlendWeight(0, weight0);
    getSkl(actor)->setSklAnimBlendWeight(1, weight1);
    getSkl(actor)->setSklAnimBlendWeight(2, weight2);
    getSkl(actor)->setSklAnimBlendWeight(3, weight3);
    getSkl(actor)->calcSklAnim();
}
void setSklAnimBlendWeightFivefold(LiveActor* actor, f32 weight0, f32 weight1, f32 weight2,
                                   f32 weight3, f32 weight4) {
    getSkl(actor)->setSklAnimBlendWeight(0, weight0);
    getSkl(actor)->setSklAnimBlendWeight(1, weight1);
    getSkl(actor)->setSklAnimBlendWeight(2, weight2);
    getSkl(actor)->setSklAnimBlendWeight(3, weight3);
    getSkl(actor)->setSklAnimBlendWeight(4, weight4);
    getSkl(actor)->calcSklAnim();
}
void setSklAnimBlendWeightSixfold(LiveActor* actor, f32 weight0, f32 weight1, f32 weight2,
                                  f32 weight3, f32 weight4, f32 weight5) {
    getSkl(actor)->setSklAnimBlendWeight(0, weight0);
    getSkl(actor)->setSklAnimBlendWeight(1, weight1);
    getSkl(actor)->setSklAnimBlendWeight(2, weight2);
    getSkl(actor)->setSklAnimBlendWeight(3, weight3);
    getSkl(actor)->setSklAnimBlendWeight(4, weight4);
    getSkl(actor)->setSklAnimBlendWeight(5, weight5);
    getSkl(actor)->calcSklAnim();
}
void setSklAnimBlendFrameAll(LiveActor* actor, f32 frame, bool isSync) {
    AnimPlayerSkl* sklAnim = getSkl(actor);

    sklAnim->setSklAnimFrame(0, frame);
    if (isSync) {
        f32 frameMax0 = sklAnim->getSklAnimFrameMax(0);
        for (s32 i = 1; i < sklAnim->getSklAnimBlendNum(); ++i) {
            if (getSkl(actor)->isSklAnimPlaying(i)) {
                f32 frameCalc = (frame / frameMax0) * sklAnim->getSklAnimFrameMax(i);
                getSkl(actor)->setSklAnimFrame(i, frameCalc);
            }
        }
    } else
        for (s32 j = 1; j < sklAnim->getSklAnimBlendNum(); ++j) {
            if (getSkl(actor)->isSklAnimPlaying(j))
                getSkl(actor)->setSklAnimFrame(j, frame);
        }
}
void setSklAnimBlendFrameRateAll(LiveActor* actor, f32 frameRate, bool isSync) {
    AnimPlayerSkl* sklAnim = getSkl(actor);

    sklAnim->setSklAnimFrameRate(0, frameRate);
    if (isSync) {
        f32 frameMax0 = sklAnim->getSklAnimFrameMax(0);
        for (s32 i = 1; i < sklAnim->getSklAnimBlendNum(); ++i) {
            if (getSkl(actor)->isSklAnimPlaying(i)) {
                f32 frameCalc = (frameRate / frameMax0) * sklAnim->getSklAnimFrameMax(i);
                getSkl(actor)->setSklAnimFrameRate(i, frameCalc);
            }
        }
    } else
        for (s32 j = 1; j < sklAnim->getSklAnimBlendNum(); ++j) {
            if (getSkl(actor)->isSklAnimPlaying(j))
                getSkl(actor)->setSklAnimFrameRate(j, frameRate);
        }
}
void startMtpAnim(LiveActor* actor, const char* animName) {
    getMtp(actor)->startAnim(animName);
    updateModelDraw(actor);
}
void startMtpAnimAndSetFrameAndStop(LiveActor* actor, const char* animName, f32 frame) {
    getMtp(actor)->startAnim(animName);
    updateModelDraw(actor);
    getMtp(actor)->setAnimFrame(frame);
    updateModelDraw(actor);
    getMtp(actor)->setAnimFrameRate(0.0);
}
void setMtpAnimFrameRate(LiveActor* actor, f32 frameRate) {
    getMtp(actor)->setAnimFrame(frameRate);
    updateModelDraw(actor);
}
bool tryStartMtpAnimIfExist(LiveActor* actor, const char* animName) {
    if (getMtp(actor)) {
        if (getMtp(actor)->isAnimExist(animName)) {
            getMtp(actor)->startAnim(animName);
            updateModelDraw(actor);
            return true;
        }
    }
    return false;
}
bool isMtpAnimExist(const LiveActor* actor, const char* animName) {
    AnimPlayerSimple* animMtp = getMtp(actor);

    if (!animMtp)
        return false;
    return getMtp(actor)->isAnimExist(animName);
}
bool tryStartMtpAnimIfNotPlaying(LiveActor* actor, const char* animName) {
    if (isEqualString(animName, getMtp(actor)->getPlayingAnimName()))
        return false;
    getMtp(actor)->startAnim(animName);
    updateModelDraw(actor);
    return true;
}
bool isMtpAnimPlaying(const LiveActor* actor, const char* animName) {
    return isEqualString(animName, getMtp(actor)->getPlayingAnimName());
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
    if (!getMtp(actor))
        return false;
    return getMtp(actor)->isAnimPlaying();
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
    getMtp(actor)->setAnimFrame(frame);
    updateModelDraw(actor);
    getMtp(actor)->setAnimFrameRate(0.0);
}
void setMtpAnimFrameAndStopEnd(LiveActor* actor) {
    f32 frameMax = getMtp(actor)->getAnimFrameMax();
    getMtp(actor)->setAnimFrame(frameMax);
    updateModelDraw(actor);
    getMtp(actor)->setAnimFrameRate(0.0);
}
void startMclAnim(LiveActor* actor, const char* animName) {
    getMcl(actor)->startAnim(animName);
}
bool tryStartMclAnimIfExist(LiveActor* actor, const char* animName) {
    if (getMcl(actor))
        if (getMcl(actor)->isAnimExist(animName)) {
            getMcl(actor)->startAnim(animName);
            return true;
        }
    return false;
}
bool isMclAnimExist(const LiveActor* actor, const char* animName) {
    if (getMcl(actor))
        return getMcl(actor)->isAnimExist(animName);
    return false;
}
bool tryStartMclAnimIfNotPlaying(LiveActor* actor, const char* animName) {
    if (isEqualString(animName, getMcl(actor)->getPlayingAnimName()))
        return false;
    getMcl(actor)->startAnim(animName);
    return true;
}
bool isMclAnimPlaying(const LiveActor* actor, const char* animName) {
    return isEqualString(animName, getMcl(actor)->getPlayingAnimName());
}
void clearMclAnim(LiveActor* actor) {
    getMcl(actor)->clearAnim();
}
bool isMclAnimExist(const LiveActor* actor) {
    return getMcl(actor) != nullptr;
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
    if (getMcl(actor))
        return getMcl(actor)->isAnimPlaying();
    return false;
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
    getMcl(actor)->startAnim(animName);
    getMcl(actor)->setAnimFrame(frame);
    getMcl(actor)->setAnimFrameRate(0.0);
}
void setMclAnimFrameAndStop(LiveActor* actor, f32 frame) {
    getMcl(actor)->setAnimFrame(frame);
    getMcl(actor)->setAnimFrameRate(0.0);
}
void setMclAnimNormalFrameAndStop(LiveActor* actor, f32 frame) {
    f32 frameMax = getMcl(actor)->getAnimFrameMax();
    getMcl(actor)->setAnimFrame(frameMax * frame);
    getMcl(actor)->setAnimFrameRate(0.0);
}
void setMclAnimFrameAndStopEnd(LiveActor* actor) {
    float frameMax = getMcl(actor)->getAnimFrameMax();
    getMcl(actor)->setAnimFrame(frameMax);
    getMcl(actor)->setAnimFrameRate(0.0);
}
void startMtsAnim(LiveActor* actor, const char* animName) {
    getMts(actor)->startAnim(animName);
}
void startMtsAnimAndSetFrameAndStop(LiveActor* actor, const char* animName, f32 frame) {
    getMts(actor)->startAnim(animName);
    getMts(actor)->setAnimFrame(frame);
    getMts(actor)->setAnimFrameRate(0.0);
}
void setMtsAnimFrame(LiveActor* actor, f32 frame) {
    getMts(actor)->setAnimFrame(frame);
}
void setMtsAnimFrameRate(LiveActor* actor, f32 frameRate) {
    getMts(actor)->setAnimFrameRate(frameRate);
}
bool tryStartMtsAnimIfExist(LiveActor* actor, const char* animName) {
    if (getMts(actor))
        if (getMts(actor)->isAnimExist(animName)) {
            getMts(actor)->startAnim(animName);
            return true;
        }
    return false;
}
bool isMtsAnimExist(const LiveActor* actor, const char* animName) {
    if (getMts(actor))
        return getMts(actor)->isAnimExist(animName);
    return false;
}
bool tryStartMtsAnimIfNotPlaying(LiveActor* actor, const char* animName) {
    if (isEqualString(animName, getMts(actor)->getPlayingAnimName()))
        return false;
    getMts(actor)->startAnim(animName);
    return true;
}
bool isMtsAnimPlaying(const LiveActor* actor, const char* animName) {
    return isEqualString(animName, getMts(actor)->getPlayingAnimName());
}
void clearMtsAnim(LiveActor* actor) {
    getMts(actor)->clearAnim();
}
bool isMtsAnimExist(LiveActor* actor) {
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
    if (getMts(actor))
        return getMts(actor)->isAnimPlaying();
    return false;
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
    getMts(actor)->setAnimFrame(frame);
    getMts(actor)->setAnimFrameRate(0.0);
}
void setMtsAnimFrameAndStopEnd(LiveActor* actor) {
    float frameMax = getMts(actor)->getAnimFrameMax();
    getMts(actor)->setAnimFrame(frameMax);
    getMts(actor)->setAnimFrameRate(0.0);
}
void createMatAnimSlotForMcl(LiveActor* actor) {
    actor->getModelKeeper()->createMatAnimForProgram(0);
}
void createMatAnimSlotForMtp(LiveActor* actor) {
    actor->getModelKeeper()->createMatAnimForProgram(1);
}
void createMatAnimSlotForMts(LiveActor* actor) {
    actor->getModelKeeper()->createMatAnimForProgram(2);
}
void startMatAnim(LiveActor* actor, const char* animName) {
    getMat(actor)->startAnim(animName);
    updateModelDraw(actor);
}
void startMatAnimAndSetFrameAndStop(LiveActor* actor, const char* animName, f32 frame) {
    getMat(actor)->startAnim(animName);
    updateModelDraw(actor);
    getMat(actor)->setAnimFrame(frame);
    getMat(actor)->setAnimFrameRate(0.0);
}
void setMatAnimFrame(LiveActor* actor, f32 frame) {
    getMat(actor)->setAnimFrame(frame);
}
void setMatAnimFrameRate(LiveActor* actor, f32 frameRate) {
    getMat(actor)->setAnimFrameRate(frameRate);
}
bool tryStartMatAnimIfExist(LiveActor* actor, const char* animName) {
    if (getMat(actor))
        if (getMat(actor)->isAnimExist(animName)) {
            getMat(actor)->startAnim(animName);
            updateModelDraw(actor);
            return true;
        }
    return false;
}
bool isMatAnimExist(const LiveActor* actor, const char* animName) {
    if (getMat(actor))
        return getMat(actor)->isAnimExist(animName);
    return false;
}
bool tryStartMatAnimIfNotPlaying(LiveActor* actor, const char* animName) {
    if (isEqualString(animName, getMat(actor)->getPlayingAnimName()))
        return false;
    getMat(actor)->startAnim(animName);
    updateModelDraw(actor);
    return true;
}
bool isMatAnimPlaying(const LiveActor* actor, const char* animName) {
    return isEqualString(animName, getMat(actor)->getPlayingAnimName());
}
void clearMatAnim(LiveActor* actor) {
    getMat(actor)->clearAnim();
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
    if (getMat(actor))
        return getMat(actor)->isAnimPlaying();
    return false;
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
    getMat(actor)->setAnimFrame(frame);
    getMat(actor)->setAnimFrameRate(0.0);
}
void setMatAnimFrameAndStopEnd(LiveActor* actor) {
    float frameMax = getMat(actor)->getAnimFrameMax();
    getMat(actor)->setAnimFrame(frameMax);
    getMat(actor)->setAnimFrameRate(0.0);
}
void startVisAnim(LiveActor* actor, const char* animName) {
    getVis(actor)->startAnim(animName);
    updateActorSystem(actor);
}
bool tryStartVisAnimIfExist(LiveActor* actor, const char* animName) {
    if (getVis(actor))
        if (getVis(actor)->isAnimExist(animName)) {
            getVis(actor)->startAnim(animName);
            updateActorSystem(actor);
            return true;
        }
    return false;
}
bool isVisAnimExist(const LiveActor* actor, const char* animName) {
    if (getVis(actor))
        return getVis(actor)->isAnimExist(animName);
    return false;
}
bool tryStartVisAnimIfNotPlaying(LiveActor* actor, const char* animName) {
    if (isEqualString(animName, getVis(actor)->getPlayingAnimName()))
        return false;
    getVis(actor)->startAnim(animName);
    updateActorSystem(actor);
    return true;
}
bool isVisAnimPlaying(const LiveActor* actor, const char* animName) {
    return isEqualString(animName, getVis(actor)->getPlayingAnimName());
}
void clearVisAnim(LiveActor* actor) {
    getVis(actor)->clearAnim();
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
    if (getVis(actor))
        return getVis(actor)->isAnimPlaying();
    return false;
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
}
void setVisAnimFrameRate(LiveActor* actor, f32 frameRate) {
    getVis(actor)->setAnimFrameRate(frameRate);
}
void setVisAnimFrameAndStop(LiveActor* actor, f32 frame) {
    getVis(actor)->setAnimFrame(frame);
    getVis(actor)->setAnimFrameRate(0.0);
}
void setVisAnimFrameAndStopEnd(LiveActor* actor) {
    float frameMax = getVis(actor)->getAnimFrameMax();
    getVis(actor)->setAnimFrame(frameMax);
    updateActorSystem(actor);
    getVis(actor)->setAnimFrameRate(0.0);
}
void startVisAnimAndSetFrameAndStop(LiveActor* actor, const char* animName, f32 frame) {
    getVis(actor)->startAnim(animName);
    updateActorSystem(actor);
    getVis(actor)->setAnimFrame(frame);
    updateActorSystem(actor);
    getVis(actor)->setAnimFrameRate(0.0);
}
void startVisAnimForAction(LiveActor* actor, const char* animName) {
    getVisForAction(actor)->startAnim(animName);
    updateActorSystem(actor);
}
bool tryStartVisAnimIfExistForAction(LiveActor* actor, const char* animName) {
    if (getVis(actor) && getVis(actor)->isAnimExist(animName)) {
        getVisForAction(actor)->startAnim(animName);
        updateActorSystem(actor);
        return true;
    }
    return false;
}
bool tryStartVisAnimIfNotPlayingForAction(LiveActor* actor, const char* animName) {
    if (isEqualString(animName, getVisForAction(actor)->getPlayingAnimName()))
        return false;
    getVisForAction(actor)->startAnim(animName);
    updateActorSystem(actor);
    return true;
}
bool isVisAnimPlayingForAction(const LiveActor* actor, const char* animName) {
    return isEqualString(animName, getVisForAction(actor)->getPlayingAnimName());
}
void clearVisAnimForAction(LiveActor* actor) {
    getVisForAction(actor)->clearAnim();
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
    if (getVisForAction(actor))
        return getVisForAction(actor)->isAnimPlaying();
    return false;
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
}
void setVisAnimFrameRateForAction(LiveActor* actor, f32 frameRate) {
    getVisForAction(actor)->setAnimFrameRate(frameRate);
}
void setVisAnimFrameAndStopForAction(LiveActor* actor, f32 frame) {
    getVisForAction(actor)->setAnimFrame(frame);
    updateActorSystem(actor);
    getVisForAction(actor)->setAnimFrameRate(0.0);
}
void setVisAnimFrameAndStopEndForAction(LiveActor* actor) {
    float frameMax = getVisForAction(actor)->getAnimFrameMax();
    getVisForAction(actor)->setAnimFrame(frameMax);
    updateActorSystem(actor);
    getVisForAction(actor)->setAnimFrameRate(0.0);
}
void startVisAnimAndSetFrameAndStopForAction(LiveActor* actor, const char* animName, f32 frame) {
    getVisForAction(actor)->startAnim(animName);
    updateActorSystem(actor);
    getVisForAction(actor)->setAnimFrame(frame);
    updateActorSystem(actor);
    getVisForAction(actor)->setAnimFrameRate(0.0);
}
void setAllAnimFrame(LiveActor* actor, f32 frame) {
    if (getSkl(actor)) {
        getSkl(actor)->setSklAnimFrame(0, frame);
    }
    if (getMtp(actor)) {
        getMtp(actor)->setAnimFrame(frame);
        updateModelDraw(actor);
    }
    if (getMcl(actor)) {
        getMcl(actor)->setAnimFrame(frame);
    }
    if (getMts(actor)) {
        getMts(actor)->setAnimFrame(frame);
    }
    if (getVis(actor)) {
        getVis(actor)->setAnimFrame(frame);
        updateActorSystem(actor);
    }
}
void setAllAnimFrameRate(LiveActor* actor, f32 frameRate) {
    if (getSkl(actor)) {
        getSkl(actor)->setSklAnimFrameRate(0, frameRate);
    }
    if (getMtp(actor)) {
        getMtp(actor)->setAnimFrameRate(frameRate);
    }
    if (getMcl(actor)) {
        getMcl(actor)->setAnimFrameRate(frameRate);
    }
    if (getMts(actor)) {
        getMts(actor)->setAnimFrameRate(frameRate);
    }
    if (getVis(actor)) {
        getVis(actor)->setAnimFrameRate(frameRate);
    }
}
void startPartialSklAnim(LiveActor* actor, const char* animName, int partCount, int value,
                         const SklAnimRetargettingInfo* info) {
    getSkl(actor)->startPartialAnim(animName, partCount, value, info);
}
void startPartialSklAnimWithInterpolate(LiveActor* actor, const char* animName, s32 partCount,
                                        s32 value, s32 interpol,
                                        const SklAnimRetargettingInfo* info) {
    AnimPlayerSkl* sklAnim = getSkl(actor);
    sklAnim->startPartialAnim(animName, partCount, value, info);
    sklAnim->prepareAnimInterpDirect(interpol);
}
void clearPartialSklAnim(LiveActor* actor, s32 index) {
    getSkl(actor)->clearPartialAnim(index);
}
void clearPartialSklAnimWithInterpolate(LiveActor* actor, s32 partIndex, s32 interpol) {
    AnimPlayerSkl* sklAnim = getSkl(actor);
    sklAnim->clearPartialAnim(partIndex);
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
    if (!getSkl(actor)->isPartialAnimAttached(index))
        return false;
    return isEqualString(partsName, getSkl(actor)->getPlayingPartialSklAnimName(index));
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
