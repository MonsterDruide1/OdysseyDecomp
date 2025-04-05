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
class AudioResourceLoadGroupInfo;
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
    al::Sequence* mSequence;
    al::GameSystemInfo* mSystemInfo;
    al::AudioSystem* mAudioSystem;
    al::AudioInfoListWithParts<al::AudioResourceLoadGroupInfo>* mAudioInfoList;
    al::AccountHolder* mAccountHolder;
    al::NetworkSystem* mNetworkSystem;
    void* field_40;
    al::HtmlViewer* mHtmlViewer;
    ProjectNfpDirector* mNfpDirector;
    al::GamePadSystem* mGamePadSystem;
    al::ApplicationMessageReceiver* mApplicationMessageReceiver;
    al::WaveVibrationHolder* mWaveVibrationHolder;
    bool mIsSinglePlay;
    GameConfigData* mGameConfigData;
    bool mIsSequenceSetupIncomplete;
};

namespace GameSystemFunction {
GameSystem* getGameSystem();
}
