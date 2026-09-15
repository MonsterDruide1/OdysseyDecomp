#pragma once

#include <basis/seadTypes.h>
#include <math/seadVectorFwd.h>

#include "Library/Area/IUseAreaObj.h"
#include "Library/Audio/IUseAudioKeeper.h"

namespace al {
class AreaObjDirector;
class AudioEffectController;
class AudioKeeper;
class ByamlIter;
class Resource;

template <typename T>
class AudioInfoListWithParts;
class BgmPlayObj;
class BgmPlayEventController;
class AudioDirector;
class PlayerHolder;

class AudioEventController : public IUseAudioKeeper, public IUseAreaObj {
public:
    AudioEventController(const AudioDirector* audioDirector, const char* stageEffectName);

    void init3D(AreaObjDirector* areaObjDirector, AudioEffectController* audioEffectController);
    void initAfterInitPlacement(const AudioDirector* audioDirector);
    void update();
    bool isEnableAudioEvent(s32 audioEvent) const;
    void finalize();
    void setPlayerHolder(const PlayerHolder* playerHolder);
    void activate();
    void deactivate();
    void activateEachAudioEvent(s32 audioEvent, bool isActivate);
    void deactivateEachAudioEvent(s32 audioEvent, bool isDeactivate);
    bool isInBgmStopArea(const char* playName);
    const char* getBgmSituationNameByAreaChecker();
    const char* getAudioEffectNameByAreaChecker();
    bool isEnableBgmChangeEvent() const;
    bool isEnableBgmSituationChangeEvent() const;
    bool isEnableAllAudioEvent() const;
    bool isDisableAllAudioEvent() const;

    AudioKeeper* getAudioKeeper() const override;
    AreaObjDirector* getAreaObjDirector() const override;

    BgmPlayEventController* getBgmPlayEventController() const { return mBgmPlayEventController; }

private:
    AudioKeeper* mAudioKeeper = nullptr;
    BgmPlayEventController* mBgmPlayEventController = nullptr;
    u8 _20[0x78];
};

static_assert(sizeof(AudioEventController) == 0x98);
}  // namespace al

namespace alAudioEventControllerFunction {
bool tryRegistBgmPlayObj(al::AudioEventController* audioEventController, al::BgmPlayObj* obj);
const char* getBgmPlayNameInCurPosition(al::AudioEventController* audioEventController,
                                        bool isStopArea);
const char* getBgmPlayNameInTargetPosition(al::AudioEventController* audioEventController,
                                           const sead::Vector3f& position);
void tryStartAndStopBgmInCurPosition(al::AudioEventController* audioEventController,
                                     bool isStopArea, bool isForceStart);
void activateAudioEventCtrl(al::AudioDirector* audioDirector);
void deactivateAudioEventCtrl(al::AudioDirector* audioDirector);
void tryActivateOrDeactivateAudioEventCtrl(al::AudioDirector* audioDirector, bool isActivate,
                                           s32 audioEvent);
}  // namespace alAudioEventControllerFunction

namespace al {
struct AudioAddonSoundArchiveInfo {
    AudioAddonSoundArchiveInfo();

    static AudioAddonSoundArchiveInfo* createInfo(const ByamlIter& iter);
    static s32 compareInfo(const AudioAddonSoundArchiveInfo* lhs,
                           const AudioAddonSoundArchiveInfo* rhs);

    const char* name = nullptr;
};

static_assert(sizeof(AudioAddonSoundArchiveInfo) == 0x8);

struct AudioSoundArchiveInfo {
    AudioSoundArchiveInfo();

    static AudioSoundArchiveInfo* createInfo(const ByamlIter& iter);

    AudioInfoListWithParts<AudioAddonSoundArchiveInfo>* addonInfo = nullptr;
};

static_assert(sizeof(AudioSoundArchiveInfo) == 0x8);

struct AudioResourceLoadingInfo {
    AudioResourceLoadingInfo();

    static AudioResourceLoadingInfo* createInfo(const Resource* resource);

    AudioSoundArchiveInfo* seInfo = nullptr;
    AudioSoundArchiveInfo* bgmInfo = nullptr;
};

static_assert(sizeof(AudioResourceLoadingInfo) == 0x10);

}  // namespace al
