#include "Library/LiveActor/ActorAnimFunction.h"

#include <basis/seadTypes.h>

#include "Library/Base/String.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/LiveActor.h"
#include "Project/Anim/AnimPlayerVisual.h"

namespace al {
void startSklAnim(LiveActor* actor, const char* string) {
    getSkl(actor)->startSklAnim(nullptr, string, nullptr, nullptr, nullptr, nullptr, nullptr);
}
void startSklAnimInterpole(LiveActor* actor, const char* string0, const char* string1) {
    getSkl(actor)->startSklAnim(string1, string0, nullptr, nullptr, nullptr, nullptr, nullptr);
}
bool tryStartSklAnimIfExist(LiveActor* actor, const char* string) {
    AnimPlayerSkl* sklAnim = getSkl(actor);

    if (sklAnim)
        if (sklAnim->isSklAnimExist(string)) {
            getSkl(actor)->startSklAnim(nullptr, string, nullptr, nullptr, nullptr, nullptr,
                                        nullptr);
            return true;
        }
    return false;
}
bool isSklAnimExist(const LiveActor* actor, const char* string) {
    if (!getSkl(actor))
        return false;

    return getSkl(actor)->isSklAnimExist(string);
}
bool tryStartSklAnimIfNotPlaying(LiveActor* actor, const char* string) {
    const char* playingName = getSkl(actor)->getPlayingSklAnimName(0);

    if (playingName && isEqualString(string, playingName))
        return false;
    getSkl(actor)->startSklAnim(nullptr, string, nullptr, nullptr, nullptr, nullptr, nullptr);
    return true;
}
bool isSklAnimPlaying(const LiveActor* actor, const char* string, s32 index) {
    const char* playingName = getSkl(actor)->getPlayingSklAnimName(index);
    return playingName && isEqualString(string, playingName);
}
void startSklAnimBlend(LiveActor* actor, const char* string0, const char* string1,
                       const char* string2, const char* string3, const char* string4,
                       const char* string5) {
    getSkl(actor)->startSklAnim(nullptr, string0, string1, string2, string3, string4, string5);
}
void startSklAnimBlendInterpole(LiveActor* actor, const char* string0, const char* string1,
                                const char* string2, const char* string3, const char* string4,
                                const char* string5, const char* string6) {
    getSkl(actor)->startSklAnim(string0, string1, string2, string3, string4, string5, string6);
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
bool isSklAnimOneTime(const LiveActor* actor, const char* string) {
    return getSkl(actor)->isSklAnimOneTime(string);
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
f32 getSklAnimFrameMax(const LiveActor* actor, const char* string) {
    return getSkl(actor)->getSklAnimFrameMax(string);
}
void setSklAnimFrame(LiveActor* actor, f32 value, s32 index) {
    getSkl(actor)->setSklAnimFrame(index, value);
}
void setSklAnimFrameRate(LiveActor* actor, f32 value, s32 index) {
    getSkl(actor)->setSklAnimFrameRate(index, value);
}
void setSklAnimFrameAndStop(LiveActor* actor, f32 value, s32 index) {
    getSkl(actor)->setSklAnimFrame(index, value);
    getSkl(actor)->setSklAnimFrameRate(index, 0.0);
}
s32 getSklAnimBlendNum(const LiveActor* actor) {
    return getSkl(actor)->getSklAnimBlendNum();
}
f32 getSklAnimBlendWeight(const LiveActor* actor, int index) {
    return getSkl(actor)->getSklAnimBlendWeight(index);
}
void setSklAnimBlendWeight(LiveActor* actor, f32 value, s32 index) {
    getSkl(actor)->setSklAnimBlendWeight(index, value);
    getSkl(actor)->calcSklAnim();
}
void setSklAnimBlendWeightDouble(LiveActor* actor, f32 value) {
    getSkl(actor)->setSklAnimBlendWeight(0, value);
    f32 value1 = 1 - value;
    getSkl(actor)->setSklAnimBlendWeight(1, value1);
    getSkl(actor)->calcSklAnim();
}
void setSklAnimBlendWeightDouble(LiveActor* actor, f32 value0, f32 value1) {
    getSkl(actor)->setSklAnimBlendWeight(0, value0);
    getSkl(actor)->setSklAnimBlendWeight(1, value1);
    getSkl(actor)->calcSklAnim();
}
void setSklAnimBlendWeightTriple(LiveActor* actor, f32 value0, f32 value1, f32 value2) {
    getSkl(actor)->setSklAnimBlendWeight(0, value0);
    getSkl(actor)->setSklAnimBlendWeight(1, value1);
    getSkl(actor)->setSklAnimBlendWeight(2, value2);
    getSkl(actor)->calcSklAnim();
}
void setSklAnimBlendWeightQuad(LiveActor* actor, f32 value0, f32 value1, f32 value2, f32 value3) {
    getSkl(actor)->setSklAnimBlendWeight(0, value0);
    getSkl(actor)->setSklAnimBlendWeight(1, value1);
    getSkl(actor)->setSklAnimBlendWeight(2, value2);
    getSkl(actor)->setSklAnimBlendWeight(3, value3);
    getSkl(actor)->calcSklAnim();
}
void setSklAnimBlendWeightFivefold(LiveActor* actor, f32 value0, f32 value1, f32 value2, f32 value3,
                                   f32 value4) {
    getSkl(actor)->setSklAnimBlendWeight(0, value0);
    getSkl(actor)->setSklAnimBlendWeight(1, value1);
    getSkl(actor)->setSklAnimBlendWeight(2, value2);
    getSkl(actor)->setSklAnimBlendWeight(3, value3);
    getSkl(actor)->setSklAnimBlendWeight(4, value4);
    getSkl(actor)->calcSklAnim();
}
void setSklAnimBlendWeightSixfold(LiveActor* actor, f32 value0, f32 value1, f32 value2, f32 value3,
                                  f32 value4, f32 value5) {
    getSkl(actor)->setSklAnimBlendWeight(0, value0);
    getSkl(actor)->setSklAnimBlendWeight(1, value1);
    getSkl(actor)->setSklAnimBlendWeight(2, value2);
    getSkl(actor)->setSklAnimBlendWeight(3, value3);
    getSkl(actor)->setSklAnimBlendWeight(4, value4);
    getSkl(actor)->setSklAnimBlendWeight(5, value5);
    getSkl(actor)->calcSklAnim();
}
void setSklAnimBlendFrameAll(LiveActor* actor, f32 value, bool isSync) {
    AnimPlayerSkl* sklAnim = getSkl(actor);

    sklAnim->setSklAnimFrame(0, value);
    if (isSync) {
        f32 frameMax0 = sklAnim->getSklAnimFrameMax(0);
        for (s32 i = 1; i < sklAnim->getSklAnimBlendNum(); ++i) {
            if (getSkl(actor)->isSklAnimPlaying(i)) {
                f32 frameVal = (value / frameMax0) * sklAnim->getSklAnimFrameMax(i);
                getSkl(actor)->setSklAnimFrame(i, frameVal);
            }
        }
    } else
        for (s32 j = 1; j < sklAnim->getSklAnimBlendNum(); ++j) {
            if (getSkl(actor)->isSklAnimPlaying(j))
                getSkl(actor)->setSklAnimFrame(j, value);
        }
}
void setSklAnimBlendFrameRateAll(LiveActor* actor, f32 value, bool isSync) {
    AnimPlayerSkl* sklAnim = getSkl(actor);

    sklAnim->setSklAnimFrameRate(0, value);
    if (isSync) {
        f32 frameMax0 = sklAnim->getSklAnimFrameMax(0);
        for (s32 i = 1; i < sklAnim->getSklAnimBlendNum(); ++i) {
            if (getSkl(actor)->isSklAnimPlaying(i)) {
                f32 frameVal = (value / frameMax0) * sklAnim->getSklAnimFrameMax(i);
                getSkl(actor)->setSklAnimFrameRate(i, frameVal);
            }
        }
    } else
        for (s32 j = 1; j < sklAnim->getSklAnimBlendNum(); ++j) {
            if (getSkl(actor)->isSklAnimPlaying(j))
                getSkl(actor)->setSklAnimFrameRate(j, value);
        }
}
void startMtpAnim(LiveActor* actor, const char* string) {
    getMtp(actor)->startAnim(string);
    updateModelDraw(actor);
}
void startMtpAnimAndSetFrameAndStop(LiveActor* actor, const char* string, f32 frameVal) {
    getMtp(actor)->startAnim(string);
    updateModelDraw(actor);
    getMtp(actor)->setAnimFrame(frameVal);
    updateModelDraw(actor);
    getMtp(actor)->setAnimFrameRate(0.0);
}
void setMtpAnimFrameRate(LiveActor* actor, f32 frameVal) {
    getMtp(actor)->setAnimFrame(frameVal);
    updateModelDraw(actor);
}
bool tryStartMtpAnimIfExist(LiveActor* actor, const char* string) {
    if (getMtp(actor)) {
        if (getMtp(actor)->isAnimExist(string)) {
            getMtp(actor)->startAnim(string);
            updateModelDraw(actor);
            return true;
        }
    }
    return false;
}
bool isMtpAnimExist(const LiveActor* actor, const char* string) {
    AnimPlayerSimple* animMtp = getMtp(actor);

    if (!animMtp)
        return false;
    return getMtp(actor)->isAnimExist(string);
}
bool tryStartMtpAnimIfNotPlaying(LiveActor* actor, const char* string) {
    if (isEqualString(string, getMtp(actor)->getPlayingAnimName()))
        return false;
    getMtp(actor)->startAnim(string);
    updateModelDraw(actor);
    return true;
}
bool isMtpAnimPlaying(const LiveActor* actor, const char* string) {
    return isEqualString(string, getMtp(actor)->getPlayingAnimName());
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
bool isMtpAnimOneTime(const LiveActor* actor, const char* string) {
    return getMtp(actor)->isAnimOneTime(string);
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
f32 getMtpAnimFrameMax(const LiveActor* actor, const char* string) {
    return getMtp(actor)->getAnimFrameMax(string);
}
void setMtpAnimFrameAndStop(LiveActor* actor, f32 frameVal) {
    getMtp(actor)->setAnimFrame(frameVal);
    updateModelDraw(actor);
    getMtp(actor)->setAnimFrameRate(0.0);
}
void setMtpAnimFrameAndStopEnd(LiveActor* actor) {
    f32 frameMax = getMtp(actor)->getAnimFrameMax();
    getMtp(actor)->setAnimFrame(frameMax);
    updateModelDraw(actor);
    getMtp(actor)->setAnimFrameRate(0.0);
}
void startMclAnim(LiveActor* actor, const char* string) {
    getMcl(actor)->startAnim(string);
}
bool tryStartMclAnimIfExist(LiveActor* actor, const char* string) {
    if (getMcl(actor))
        if (getMcl(actor)->isAnimExist(string)) {
            getMcl(actor)->startAnim(string);
            return true;
        }
    return false;
}
bool isMclAnimExist(const LiveActor* actor, const char* string) {
    if (getMcl(actor))
        return getMcl(actor)->isAnimExist(string);
    return false;
}
bool tryStartMclAnimIfNotPlaying(LiveActor* actor, const char* string) {
    if (isEqualString(string, getMcl(actor)->getPlayingAnimName()))
        return false;
    getMcl(actor)->startAnim(string);
    return true;
}
bool isMclAnimPlaying(const LiveActor* actor, const char* string) {
    return isEqualString(string, getMcl(actor)->getPlayingAnimName());
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
bool isMclAnimOneTime(const LiveActor* actor, const char* string) {
    return getMcl(actor)->isAnimOneTime(string);
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
f32 getMclAnimFrameMax(const LiveActor* actor, const char* string) {
    return getMcl(actor)->getAnimFrameMax(string);
}
void setMclAnimFrame(LiveActor* actor, f32 frameVal) {
    getMcl(actor)->setAnimFrame(frameVal);
}
void setMclAnimFrameRate(LiveActor* actor, f32 frameVal) {
    getMcl(actor)->setAnimFrameRate(frameVal);
}
void startMclAnimAndSetFrameAndStop(LiveActor* actor, const char* string, f32 frameVal) {
    getMcl(actor)->startAnim(string);
    getMcl(actor)->setAnimFrame(frameVal);
    getMcl(actor)->setAnimFrameRate(0.0);
}
void setMclAnimFrameAndStop(LiveActor* actor, f32 frameVal) {
    getMcl(actor)->setAnimFrame(frameVal);
    getMcl(actor)->setAnimFrameRate(0.0);
}
void setMclAnimNormalFrameAndStop(LiveActor* actor, f32 frameVal) {
    f32 frameMax = getMcl(actor)->getAnimFrameMax();
    getMcl(actor)->setAnimFrame(frameMax * frameVal);
    getMcl(actor)->setAnimFrameRate(0.0);
}
void setMclAnimFrameAndStopEnd(LiveActor* actor) {
    float frameMax = getMcl(actor)->getAnimFrameMax();
    getMcl(actor)->setAnimFrame(frameMax);
    getMcl(actor)->setAnimFrameRate(0.0);
}
void startMtsAnim(LiveActor* actor, const char* string) {
    getMts(actor)->startAnim(string);
}
void startMtsAnimAndSetFrameAndStop(LiveActor* actor, const char* string, f32 frameVal) {
    getMts(actor)->startAnim(string);
    getMts(actor)->setAnimFrame(frameVal);
    getMts(actor)->setAnimFrameRate(0.0);
}
void setMtsAnimFrame(LiveActor* actor, f32 frameVal) {
    getMts(actor)->setAnimFrame(frameVal);
}
void setMtsAnimFrameRate(LiveActor* actor, f32 frameVal) {
    getMts(actor)->setAnimFrameRate(frameVal);
}
bool tryStartMtsAnimIfExist(LiveActor* actor, const char* string) {
    if (getMts(actor))
        if (getMts(actor)->isAnimExist(string)) {
            getMts(actor)->startAnim(string);
            return true;
        }
    return false;
}
bool isMtsAnimExist(const LiveActor* actor, const char* string) {
    if (getMts(actor))
        return getMts(actor)->isAnimExist(string);
    return false;
}
bool tryStartMtsAnimIfNotPlaying(LiveActor* actor, const char* string) {
    if (isEqualString(string, getMts(actor)->getPlayingAnimName()))
        return false;
    getMts(actor)->startAnim(string);
    return true;
}
bool isMtsAnimPlaying(const LiveActor* actor, const char* string) {
    return isEqualString(string, getMts(actor)->getPlayingAnimName());
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
bool isMtsAnimOneTime(const LiveActor* actor, const char* string) {
    return getMts(actor)->isAnimOneTime(string);
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
f32 getMtsAnimFrameMax(const LiveActor* actor, const char* string) {
    return getMts(actor)->getAnimFrameMax(string);
}
void setMtsAnimFrameAndStop(LiveActor* actor, f32 frameVal) {
    getMts(actor)->setAnimFrame(frameVal);
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
void startMatAnim(LiveActor* actor, const char* string) {
    getMat(actor)->startAnim(string);
    updateModelDraw(actor);
}
void startMatAnimAndSetFrameAndStop(LiveActor* actor, const char* string, f32 frameVal) {
    getMat(actor)->startAnim(string);
    updateModelDraw(actor);
    getMat(actor)->setAnimFrame(frameVal);
    getMat(actor)->setAnimFrameRate(0.0);
}
void setMatAnimFrame(LiveActor* actor, f32 frameVal) {
    getMat(actor)->setAnimFrame(frameVal);
}
void setMatAnimFrameRate(LiveActor* actor, f32 frameVal) {
    getMat(actor)->setAnimFrameRate(frameVal);
}
bool tryStartMatAnimIfExist(LiveActor* actor, const char* string) {
    if (getMat(actor))
        if (getMat(actor)->isAnimExist(string)) {
            getMat(actor)->startAnim(string);
            updateModelDraw(actor);
            return true;
        }
    return false;
}
bool isMatAnimExist(const LiveActor* actor, const char* string) {
    if (getMat(actor))
        return getMat(actor)->isAnimExist(string);
    return false;
}
bool tryStartMatAnimIfNotPlaying(LiveActor* actor, const char* string) {
    if (isEqualString(string, getMat(actor)->getPlayingAnimName()))
        return false;
    getMat(actor)->startAnim(string);
    updateModelDraw(actor);
    return true;
}
bool isMatAnimPlaying(const LiveActor* actor, const char* string) {
    return isEqualString(string, getMat(actor)->getPlayingAnimName());
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
bool isMatAnimOneTime(const LiveActor* actor, const char* string) {
    return getMat(actor)->isAnimOneTime(string);
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
f32 getMatAnimFrameMax(const LiveActor* actor, const char* string) {
    return getMat(actor)->getAnimFrameMax(string);
}
void setMatAnimFrameAndStop(LiveActor* actor, f32 frameVal) {
    getMat(actor)->setAnimFrame(frameVal);
    getMat(actor)->setAnimFrameRate(0.0);
}
void setMatAnimFrameAndStopEnd(LiveActor* actor) {
    float frameMax = getMat(actor)->getAnimFrameMax();
    getMat(actor)->setAnimFrame(frameMax);
    getMat(actor)->setAnimFrameRate(0.0);
}
void startVisAnim(LiveActor* actor, const char* string) {
    getVis(actor)->startAnim(string);
    updateActorSystem(actor);
}
bool tryStartVisAnimIfExist(LiveActor* actor, const char* string) {
    if (getVis(actor))
        if (getVis(actor)->isAnimExist(string)) {
            getVis(actor)->startAnim(string);
            updateActorSystem(actor);
            return true;
        }
    return false;
}
bool isVisAnimExist(const LiveActor* actor, const char* string) {
    if (getVis(actor))
        return getVis(actor)->isAnimExist(string);
    return false;
}
bool tryStartVisAnimIfNotPlaying(LiveActor* actor, const char* string) {
    if (isEqualString(string, getVis(actor)->getPlayingAnimName()))
        return false;
    getVis(actor)->startAnim(string);
    updateActorSystem(actor);
    return true;
}
bool isVisAnimPlaying(const LiveActor* actor, const char* string) {
    return isEqualString(string, getVis(actor)->getPlayingAnimName());
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
bool isVisAnimOneTime(const LiveActor* actor, const char* string) {
    return getVis(actor)->isAnimOneTime(string);
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
f32 getVisAnimFrameMax(const LiveActor* actor, const char* string) {
    return getVis(actor)->getAnimFrameMax(string);
}
void setVisAnimFrame(LiveActor* actor, f32 frameVal) {
    getVis(actor)->setAnimFrame(frameVal);
}
void setVisAnimFrameRate(LiveActor* actor, f32 frameVal) {
    getVis(actor)->setAnimFrameRate(frameVal);
}
void setVisAnimFrameRateAndStop(LiveActor* actor, f32 frameVal) {
    getVis(actor)->setAnimFrame(frameVal);
    getVis(actor)->setAnimFrameRate(0.0);
}
void setVisAnimFrameAndStopEnd(LiveActor* actor) {
    float frameMax = getVis(actor)->getAnimFrameMax();
    getVis(actor)->setAnimFrame(frameMax);
    updateActorSystem(actor);
    getVis(actor)->setAnimFrameRate(0.0);
}
void startVisAnimAndSetFrameAndStop(LiveActor* actor, const char* string, f32 frameVal) {
    getVis(actor)->startAnim(string);
    updateActorSystem(actor);
    getVis(actor)->setAnimFrame(frameVal);
    updateActorSystem(actor);
    getVis(actor)->setAnimFrameRate(0.0);
}
void startVisAnimForAction(LiveActor* actor, const char* string) {
    getVisForAction(actor)->startAnim(string);
    updateActorSystem(actor);
}
bool tryStartVisAnimIfExistForAction(LiveActor* actor, const char* string) {
    if (getVis(actor))
        if (getVis(actor)->isAnimExist(string)) {
            getVisForAction(actor)->startAnim(string);
            updateActorSystem(actor);
            return true;
        }
    return false;
}
bool tryStartVisAnimIfNotPlayingForAction(LiveActor* actor, const char* string) {
    if (isEqualString(string, getVisForAction(actor)->getPlayingAnimName()))
        return false;
    getVisForAction(actor)->startAnim(string);
    updateActorSystem(actor);
    return true;
}
bool isVisAnimPlayingForAction(const LiveActor* actor, const char* string) {
    return isEqualString(string, getVisForAction(actor)->getPlayingAnimName());
}
void clearVisAnimForAction(LiveActor* actor) {
    getVisForAction(actor)->clearAnim();
}
bool isVisAnimEndForAction(const LiveActor* actor) {
    return getVisForAction(actor)->isAnimEnd();
}
bool isVisAnimOneTimeForAction(const LiveActor* actor, const char* string) {
    return getVisForAction(actor)->isAnimOneTime(string);
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
void setVisAnimFrameForAction(LiveActor* actor, f32 frameVal) {
    getVisForAction(actor)->setAnimFrame(frameVal);
}
void setVisAnimFrameRateForAction(LiveActor* actor, f32 frameVal) {
    getVisForAction(actor)->setAnimFrameRate(frameVal);
}
void setVisAnimFrameAndStopForAction(LiveActor* actor, f32 frameVal) {
    getVisForAction(actor)->setAnimFrame(frameVal);
    updateActorSystem(actor);
    getVisForAction(actor)->setAnimFrameRate(0.0);
}
void setVisAnimFrameAndStopEndForAction(LiveActor* actor) {
    float frameMax = getVisForAction(actor)->getAnimFrameMax();
    getVisForAction(actor)->setAnimFrame(frameMax);
    updateActorSystem(actor);
    getVisForAction(actor)->setAnimFrameRate(0.0);
}
void startVisAnimAndSetFrameAndStopForAction(LiveActor* actor, const char* string, f32 frameVal) {
    getVisForAction(actor)->startAnim(string);
    updateActorSystem(actor);
    getVisForAction(actor)->setAnimFrame(frameVal);
    updateActorSystem(actor);
    getVisForAction(actor)->setAnimFrameRate(0.0);
}
void setAllAnimFrame(LiveActor* actor, f32 frameVal) {
    if (getSkl(actor)) {
        getSkl(actor)->setSklAnimFrame(0, frameVal);
    }
    if (getMtp(actor)) {
        getMtp(actor)->setAnimFrame(frameVal);
        updateModelDraw(actor);
    }
    if (getMcl(actor)) {
        getMcl(actor)->setAnimFrame(frameVal);
    }
    if (getMts(actor)) {
        getMts(actor)->setAnimFrame(frameVal);
    }
    if (getVis(actor)) {
        getVis(actor)->setAnimFrame(frameVal);
        updateActorSystem(actor);
    }
}
void setAllAnimFrameRate(LiveActor* actor, f32 frameVal) {
    if (getSkl(actor)) {
        getSkl(actor)->setSklAnimFrameRate(0, frameVal);
    }
    if (getMtp(actor)) {
        getMtp(actor)->setAnimFrameRate(frameVal);
    }
    if (getMcl(actor)) {
        getMcl(actor)->setAnimFrameRate(frameVal);
    }
    if (getMts(actor)) {
        getMts(actor)->setAnimFrameRate(frameVal);
    }
    if (getVis(actor)) {
        getVis(actor)->setAnimFrameRate(frameVal);
    }
}
}  // namespace al
