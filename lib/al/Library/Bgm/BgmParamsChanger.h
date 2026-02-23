#pragma once

#include <basis/seadTypes.h>
#include <nn/atk/AuxBus.h>

#include "Project/Bgm/BgmInfo.h"

namespace al {
class DspLinearValueController;
class DspSinValueController;
struct BgmRegionChangeParams;

class IBgmParamsChanger {
public:
    virtual void resetForReuse() = 0;
    virtual bool isActivate() const = 0;
    virtual bool isPaused() const = 0;
    virtual void changeVolume(f32 target, s32 stepCount) = 0;
    virtual void changePitchShift(f32 target, s32 stepCount) = 0;
    virtual void changePitchShiftModulation(f32 target, f32 frequency, s32 stepCount) = 0;
    virtual void changeLpf(f32 target, s32 stepCount) = 0;
    virtual void changeBiquadFilter(f32 target, s32 stepCount) = 0;
    virtual void changeEffectSend(nn::atk::AuxBus bus, f32 target, s32 stepCount) = 0;
    virtual void changeTrackVolume(s32 track, f32 target, s32 stepCount) = 0;
    virtual void changeDefaultParams(s32 stepCount) = 0;
    virtual bool isEnableRegionChange() const = 0;
    virtual void setRegionChangeParams(const BgmRegionChangeParams& params) = 0;
    virtual const char* getSituationName() const = 0;
    virtual const char* getSubSituationName() const = 0;
    virtual void setSituationName(const char* name) = 0;
    virtual void setSubSituationName(const char* name) = 0;
    virtual f32 getCurVolume() const = 0;
    virtual f32 getCurPitchShift() const = 0;
    virtual f32 getCurPitchShiftModulation() const = 0;
    virtual f32 getCurLpfCutOff() const = 0;
    virtual f32 getBiquadFilterValue() const = 0;
    virtual f32 getEffectSend() const = 0;
    virtual nn::atk::AuxBus getEffectBusId() const = 0;
    virtual f32 getCurTrackVolume(s32 track) const = 0;
    virtual const BgmRegionChangeParams& getRegionChangeParams() const = 0;
};

class BgmParamsChanger : public IBgmParamsChanger {
public:
    BgmParamsChanger();
    void update();
    bool isReachedTargetAllController() const;
    void resetParams();
    void activate(const char* situationName, const char* subSituationName, bool unk);
    void deactivate(bool isReuse);
    void tryDeactivate(bool isReuse);
    void resetForReuse() override;
    void changeVolume(f32 target, s32 stepCount) override;
    void changePitchShift(f32 target, s32 stepCount) override;
    void changeLpf(f32 target, s32 stepCount) override;
    void changeBiquadFilter(f32 target, s32 stepCount) override;
    void changePitchShiftModulation(f32 target, f32 frequency, s32 stepCount) override;
    void changeEffectSend(nn::atk::AuxBus bus, f32 target, s32 stepCount) override;
    void changeTrackVolume(s32 track, f32 target, s32 stepCount) override;
    void changeDefaultParams(s32 stepCount) override;
    void forceDeactivate();
    f32 getCurVolume() const override;
    f32 getCurPitchShift() const override;
    f32 getCurPitchShiftModulation() const override;
    f32 getCurLpfCutOff() const override;
    f32 getBiquadFilterValue() const override;
    f32 getEffectSend() const override;
    nn::atk::AuxBus getEffectBusId() const override;
    f32 getCurTrackVolume(s32 track) const override;

    bool isActivate() const override { return mIsActivate; }

    bool isPaused() const override { return mIsPaused; }

    bool isEnableRegionChange() const override { return mIsEnableRegionChange; }

    void setRegionChangeParams(const BgmRegionChangeParams& params) override {
        mIsEnableRegionChange = true;
        mRegionChangeParams = params;
    }

    const char* getSituationName() const override { return mSituationName; }

    const char* getSubSituationName() const override { return mSubSituationName; }

    void setSituationName(const char* name) override { mSituationName = name; }

    void setSubSituationName(const char* name) override { mSubSituationName = name; }

    const BgmRegionChangeParams& getRegionChangeParams() const override {
        return mRegionChangeParams;
    }

private:
    DspLinearValueController* mVolume = nullptr;
    DspLinearValueController* mPitchShift = nullptr;
    DspLinearValueController* mLpf = nullptr;
    DspLinearValueController* mBiquadFilter = nullptr;
    DspLinearValueController* mEffectSend = nullptr;
    DspSinValueController* mPitchShiftModulation = nullptr;
    nn::atk::AuxBus mEffectBus = nn::atk::AuxBus::AuxBus_A;
    DspLinearValueController* mTrackVolume[6];
    bool mIsEnableRegionChange = false;
    BgmRegionChangeParams mRegionChangeParams;
    bool mIsActivate = false;
    bool mIsPaused = false;
    bool mIsReuse = false;
    bool _87 = false;
    const char* mSituationName = nullptr;
    const char* mSubSituationName = nullptr;
};

static_assert(sizeof(BgmParamsChanger) == 0x98);

}  // namespace al
