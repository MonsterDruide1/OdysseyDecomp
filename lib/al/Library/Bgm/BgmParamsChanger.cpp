#include "Library/Bgm/BgmParamsChanger.h"

#include "Library/Bgm/DspValueController.h"

namespace al {
BgmParamsChanger::BgmParamsChanger() {
    mVolume = new DspLinearValueController(1.0f);
    mPitchShift = new DspLinearValueController(0.0f);
    mLpf = new DspLinearValueController(0.0f);
    mBiquadFilter = new DspLinearValueController(0.0f);
    mEffectSend = new DspLinearValueController(0.0f);
    mPitchShiftModulation = new DspSinValueController(60.0f, 0.0f);

    for (s32 i = 0; i < 6; i++)
        mTrackVolume[i] = new DspLinearValueController(i == 0 ? 1.0f : 0.0f);
}

void BgmParamsChanger::update() {
    mVolume->update();
    mPitchShift->update();
    mPitchShiftModulation->update();
    mLpf->update();
    mBiquadFilter->update();
    mEffectSend->update();

    for (s32 i = 0; i < 6; i++)
        mTrackVolume[i]->update();

    if (mIsReuse && isReachedTargetAllController())
        deactivate(false);
}

bool BgmParamsChanger::isReachedTargetAllController() const {
    if (mVolume->isReachedTarget() && mPitchShift->isReachedTarget() &&
        mPitchShiftModulation->isReachedTarget() && mLpf->isReachedTarget() &&
        mBiquadFilter->isReachedTarget() && mEffectSend->isReachedTarget()) {
        for (s32 i = 0; i < 6; i++)
            if (!mTrackVolume[i]->isReachedTarget())
                return false;
        return true;
    }
    return false;
}

void BgmParamsChanger::resetParams() {
    mIsActivate = false;
    mIsPaused = false;
    mSubSituationName = nullptr;
    mSituationName = nullptr;
    _87 = 0;
}

// TODO: Rename unk
void BgmParamsChanger::activate(const char* situationName, const char* subSituationName, bool unk) {
    mIsActivate = true;
    mSituationName = situationName;
    mSubSituationName = subSituationName;
    _87 = unk;
    mIsReuse = false;
    changeDefaultParams(0);
}

void BgmParamsChanger::deactivate(bool isReuse) {
    mIsReuse = isReuse;
    if (!isReuse)
        resetParams();
}

void BgmParamsChanger::tryDeactivate(bool isReuse) {
    if (mIsActivate)
        deactivate(isReuse);
}

void BgmParamsChanger::resetForReuse() {
    mIsReuse = false;
}

void BgmParamsChanger::changeVolume(f32 target, s32 stepCount) {
    if (mIsActivate && !mIsPaused)
        mVolume->changeTarget(target, stepCount);
}

void BgmParamsChanger::changePitchShift(f32 target, s32 stepCount) {
    if (mIsActivate && !mIsPaused)
        mPitchShift->changeTarget(target, stepCount);
}

void BgmParamsChanger::changeLpf(f32 target, s32 stepCount) {
    if (mIsActivate && !mIsPaused)
        mLpf->changeTarget(target, stepCount);
}

void BgmParamsChanger::changeBiquadFilter(f32 target, s32 stepCount) {
    if (mIsActivate && !mIsPaused)
        mBiquadFilter->changeTarget(target, stepCount);
}

void BgmParamsChanger::changePitchShiftModulation(f32 target, f32 frequency, s32 stepCount) {
    if (mIsActivate && !mIsPaused) {
        mPitchShiftModulation->changeTarget(target, stepCount);
        mPitchShiftModulation->changeFreq(frequency);
    }
}

void BgmParamsChanger::changeEffectSend(nn::atk::AuxBus bus, f32 target, s32 stepCount) {
    if (mIsActivate && !mIsPaused) {
        mEffectBus = bus;
        mEffectSend->changeTarget(target, stepCount);
    }
}

void BgmParamsChanger::changeTrackVolume(s32 track, f32 target, s32 stepCount) {
    if (mIsActivate && !mIsPaused)
        mTrackVolume[track]->changeTarget(target, stepCount);
}

void BgmParamsChanger::changeDefaultParams(s32 stepCount) {
    mVolume->changeTarget(1.0f, stepCount);
    mPitchShift->changeTarget(0.0f, stepCount);
    mLpf->changeTarget(0.0f, stepCount);
    mBiquadFilter->changeTarget(0.0f, stepCount);
    mEffectSend->changeTarget(0.0f, stepCount);
    mPitchShiftModulation->changeTarget(0.0f, stepCount);

    for (s32 i = 0; i < 6; i++)
        mTrackVolume[i]->changeTarget(i == 0 ? 1.0f : 0.0f, stepCount);
}

void BgmParamsChanger::forceDeactivate() {
    mVolume->changeTarget(mVolume->getCurrent(), 0);
    mPitchShift->changeTarget(mPitchShift->getCurrent(), 0);
    mPitchShiftModulation->changeTarget(mPitchShiftModulation->getCurrent(), 0);
    mLpf->changeTarget(mLpf->getCurrent(), 0);
    mBiquadFilter->changeTarget(mBiquadFilter->getCurrent(), 0);
    mEffectSend->changeTarget(mEffectSend->getCurrent(), 0);

    for (s32 i = 0; i < 6; i++)
        mTrackVolume[i]->changeTarget(mTrackVolume[i]->getCurrent(), 0);
}

f32 BgmParamsChanger::getCurVolume() const {
    return mVolume->getCurrent();
}

f32 BgmParamsChanger::getCurPitchShift() const {
    return mPitchShift->getCurrent();
}

f32 BgmParamsChanger::getCurPitchShiftModulation() const {
    return mPitchShiftModulation->getCurrent();
}

f32 BgmParamsChanger::getCurLpfCutOff() const {
    return mLpf->getCurrent();
}

f32 BgmParamsChanger::getBiquadFilterValue() const {
    return mBiquadFilter->getCurrent();
}

f32 BgmParamsChanger::getEffectSend() const {
    return mEffectSend->getCurrent();
}

nn::atk::AuxBus BgmParamsChanger::getEffectBusId() const {
    return mEffectBus;
}

f32 BgmParamsChanger::getCurTrackVolume(s32 track) const {
    return mTrackVolume[track]->getCurrent();
}

}  // namespace al
