#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Area/AreaObj.h"
#include "Library/Area/IUseAreaObj.h"
#include "Library/HostIO/HioNode.h"

namespace al {

class AudioDirector;
class ModelKeeper;
class CameraDirector;
class AreaObjDirector;
class AudioEventController;
class AudioEffectController;
class AudioRequestKeeperSyncedBgm;
class AudioMic;
class PlayerHolder;
class SeKeeper;
class BgmKeeper;

class AudioKeeper : public HioNode {
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

    AudioEventController* getAudioEventController() const { return mAudioEventController; }

    AudioEffectController* getAudioEffectController() const { return mAudioEffectController; }

    AudioRequestKeeperSyncedBgm* getAudioRequestKeeperSyncedBgm() const {
        return mAudioRequestKeeperSyncedBgm;
    }

    SeKeeper* getSeKeeper() const { return mSeKeeper; }

    BgmKeeper* getBgmKeeper() const { return mBgmKeeper; }

    AudioMic* getAudioMic() const { return mAudioMic; }

private:
    AudioEventController* mAudioEventController;
    AudioEffectController* mAudioEffectController;
    AudioRequestKeeperSyncedBgm* mAudioRequestKeeperSyncedBgm;
    SeKeeper* mSeKeeper;
    BgmKeeper* mBgmKeeper;
    AudioMic* mAudioMic;
};

static_assert(sizeof(AudioKeeper) == 0x38);

class AudioGeneralPurposeAreaChecker : public HioNode, public IUseAreaObj {
public:
    AudioGeneralPurposeAreaChecker(const char* areaName);

    void init(AreaObjDirector* areaObjDirector);

    void reset();
    void update();

    bool tryFindAreaObjPlayerOne(AreaObj**) const;

    bool isInArea() const;
    bool isInvaridByOneTime() const;

    void setPlayerHolder(const PlayerHolder* playerHolder);

    s32 getIntArgInCurArea(const char*) const;
    f32 getFloatArgInCurArea(const char*) const;
    bool getBoolArgInCurArea(const char*) const;

    const char* getStringArgInCurArea(const char*) const;
    const char* getStringArgInCurAreaWithAreaCheck(const char*) const;

    s32 getIntArgInPastArea(const char*) const;
    const char* getStringArgInPastArea(const char*) const;

    bool tryGetIntArgInCurArea(s32*, const char*) const;
    bool tryGetStringArgInCurArea(const char**, const char*) const;

    AreaObjDirector* getAreaObjDirector() const override { return mAreaObjDirector; }

    bool isEnteredArea() const { return mIsEnteredArea; }

    bool isExitedArea() const { return mIsExitedArea; }

private:
    const char* mAreaName;

    const AreaObj* mCurArea = nullptr;
    const AreaObj* mPrevArea = nullptr;

    u8* unk = nullptr;  // Unused? Also (seemingly) unused in RedCarpet.

    AreaObjDirector* mAreaObjDirector = nullptr;
    const PlayerHolder* mPlayerHolder = nullptr;

    bool mIsEnteredArea = false;
    bool mIsExitedArea = false;
    bool mIsAreaChanged = false;
    bool mIsCurAreaActive = false;
};

static_assert(sizeof(AudioGeneralPurposeAreaChecker) == 0x40);

}  // namespace al

namespace alAudioKeeperFunction {
al::AudioKeeper* createAudioKeeper(const al::AudioDirector*);
al::AudioKeeper* createAudioKeeper(const al::AudioDirector*, const char*, const char*);
}  // namespace alAudioKeeperFunction
