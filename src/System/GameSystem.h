#pragma once

#include "Library/Nerve/NerveExecutor.h"

namespace al {
class Sequence;
struct GameSystemInfo;
class AudioSystem;
template <typename T>
class AudioInfoListWithParts;
class AccountHolder;
class NetworkSystem;
class HtmlViewer;
class GamePadSystem;
class ApplicationMessageReceiver;
class WaveVibrationHolder;
struct AudioResourceLoadGroupInfo;
}  // namespace al

class GameConfigData;
class ProjectNfpDirector;

class GameSystem : public al::NerveExecutor {
public:
    GameSystem();
    virtual void init();
    virtual void movement();
    void setPadName();
    bool tryChangeSequence(const char*);
    void drawMain();

    void exePlay();

private:
    al::Sequence* mSequence = nullptr;
    al::GameSystemInfo* mSystemInfo = nullptr;
    al::AudioSystem* mAudioSystem = nullptr;
    al::AudioInfoListWithParts<al::AudioResourceLoadGroupInfo>* mAudioInfoList = nullptr;
    al::AccountHolder* mAccountHolder = nullptr;
    al::NetworkSystem* mNetworkSystem = nullptr;
    void* field_40 = nullptr;
    al::HtmlViewer* mHtmlViewer = nullptr;
    ProjectNfpDirector* mNfpDirector = nullptr;
    al::GamePadSystem* mGamePadSystem = nullptr;
    al::ApplicationMessageReceiver* mApplicationMessageReceiver = nullptr;
    al::WaveVibrationHolder* mWaveVibrationHolder = nullptr;
    bool mIsSinglePlay = false;
    GameConfigData* mGameConfigData = nullptr;
    bool mIsSequenceSetupIncomplete = false;
};

namespace GameSystemFunction {
GameSystem* getGameSystem();
}  // namespace GameSystemFunction
