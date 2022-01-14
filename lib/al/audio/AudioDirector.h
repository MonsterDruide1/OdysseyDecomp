#pragma once

#include <container/seadPtrArray.h>
#include "al/audio/IAudioSystemPause.h"
#include "al/hio/HioNode.h"
#include "al/iuse/IUseAreaObj.h"
#include "al/util/OtherUtil.h"

namespace aal {
class AudioFrameProcessMgr;
class IAudioFrameProcess;
}  // namespace aal

namespace al {
class SeDirector;
class BgmDirector;
class AudioMic;
class AudioEventController;
class AudioRequest;
class AudioRequestKeeperSyncedBgm;
class AudioSystemInfo;
class AreaObjDirector;
class AudioDuckingDirector;
class AudioEffectController;
class AudioDirectorInitInfo;

class AudioDirector : public IUseAreaObj, public HioNode, public IAudioSystemPause {
public:
    class PauseSystemEntry {
        const char* mName;
        bool _8;
        bool _9;
    };

    AudioDirector();
    void init(const al::AudioDirectorInitInfo& info);
    void init3D(const al::AudioDirectorInitInfo& info);
    void initAfterInitPlacement();
    void initMic(al::AudioMic* mic);
    void setDependentModule(al::AudioDirector* director);
    void updatePre();
    void updatePost();
    void startFinalizeUnsafeModuleInParallelThread();
    void updateFinalizeUnsafeModuleInParallelThread();
    bool isFinalizedUnsafeModuleInParallelThread();
    void finalize();
    bool isSystemPauseNow() const;
    void setPlayerHolder(const al::PlayerHolder* player_holder);
    void addAudiioFrameProccess(aal::IAudioFrameProcess* process);
    void removeAudiioFrameProccess(aal::IAudioFrameProcess* process);
    void* getAudioMaximizer() const;  // unknown return type
    bool isPpausedSystem(const char* name) const;

    void pauseSystem(bool, const char*, bool, float, bool, bool, bool) override;
    AreaObjDirector* getAreaObjDirector() const override;

private:
    SeDirector* mSeDirector;
    BgmDirector* mBgmDirector;
    AudioMic* mAudioMic;
    AudioEventController* mAudioEventController;
    AudioRequestKeeperSyncedBgm* mAudioRequestKeeperSyncedBgm;
    const AudioSystemInfo* mAudioSystemInfo;
    AreaObjDirector* mAreaObjDirector;
    AudioDuckingDirector* mAudioDuckingDirector;
    sead::PtrArray<PauseSystemEntry> mPauseSystemEntries;
    aal::AudioFrameProcessMgr* mAudioFrameProcessMgr;
    AudioEffectController* mAudioEffectController;
};

}  // namespace al
