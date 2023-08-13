#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include "Library/HostIO/HioNode.h"

namespace al {
class AudioDirector;
class ModelKeeper;
class CameraDirector;
class AudioEventController;
class AudioEffectController;
class AudioRequestKeeperSyncedBgm;
class AudioMic;
class SeKeeper;
class BgmKeeper;

class AudioKeeper : public IUseHioNode {
public:
    AudioKeeper(const AudioDirector*);
    virtual ~AudioKeeper();

    void initSeKeeper(const AudioDirector*, const char*, const sead::Vector3f*,
                      const sead::Matrix34f*, const ModelKeeper*, CameraDirector*);
    void initBgmKeeper(const AudioDirector*, const char*);
    void validate();
    void invalidate();
    void startClipped();
    void endClipped();
    void appear();
    void kill();

    AudioEventController* getAudioEventController() const { return mAudioEventController; };
    AudioEffectController* getAudioEffectController() const { return mAudioEffectController; };
    AudioRequestKeeperSyncedBgm* getAudioRequestKeeperSyncedBgm() const {
        return mAudioRequestKeeperSyncedBgm;
    };
    SeKeeper* getSeKeeper() const { return mSeKeeper; };
    BgmKeeper* getBgmKeeper() const { return mBgmKeeper; };
    AudioMic* getAudioMic() const { return mAudioMic; };

private:
    AudioEventController* mAudioEventController;
    AudioEffectController* mAudioEffectController;
    AudioRequestKeeperSyncedBgm* mAudioRequestKeeperSyncedBgm;
    SeKeeper* mSeKeeper;
    BgmKeeper* mBgmKeeper;
    AudioMic* mAudioMic;
};
static_assert(sizeof(AudioKeeper) == 0x38);

}  // namespace al
