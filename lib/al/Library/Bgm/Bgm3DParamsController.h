#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

#include "Library/Audio/IUseAudioKeeper.h"

namespace al {
class AudioDirector;
class AudioKeeper;
class IBgmParamsChanger;

class Bgm3DParamsController : public IUseAudioKeeper {
public:
    Bgm3DParamsController(const AudioDirector* audioDirector);

    void update();
    void finalize();
    void start3D();
    void end3D();
    void change3DParams(f32 volume, f32 pitch, f32 lpfFreq, f32 auxBusSend, bool isForce);
    void activate();
    void deactivate();

    AudioKeeper* getAudioKeeper() const override;

    bool isActive() const { return mIsActive; }

private:
    sead::PtrArray<IBgmParamsChanger>* mParamsChangers = nullptr;
    AudioKeeper* mAudioKeeper = nullptr;
    bool mIs3DStarted = false;
    bool mIsActive = false;
    u8 _1a[6] = {};
};

static_assert(sizeof(Bgm3DParamsController) == 0x20);

}  // namespace al
