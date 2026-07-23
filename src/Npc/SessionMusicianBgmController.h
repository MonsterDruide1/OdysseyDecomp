#pragma once

#include "Library/Audio/System/AudioKeeper.h"
#include "Library/Nerve/NerveExecutor.h"

namespace al {
struct ActorInitInfo;
class AudioGeneralPurposeAreaChecker;
class BgmBeatCounter;
class ISceneObj;
class LiveActor;
}  // namespace al

class CollectBgmPlayer;

class SessionMusicianBgmController : public al::NerveExecutor {
public:
    SessionMusicianBgmController(al::LiveActor* actor, const al::ActorInitInfo& initInfo,
                                 bool forceFullBand);
    ~SessionMusicianBgmController() override = default;

    void exeWait();
    void exeInArea();
    void exeOutArea();

    bool tryStartBgmSituation();

private:
    al::LiveActor* mActor;
    al::AudioGeneralPurposeAreaChecker* mAudioChecker = nullptr;
    const char* mPrevBgmName = nullptr;
    bool mIsFullBandPerformance = false;
    al::BgmBeatCounter* mBeatCounter = nullptr;
    CollectBgmPlayer* mCollectBgmPlayer = nullptr;
};

static_assert(sizeof(SessionMusicianBgmController) == 0x40);
