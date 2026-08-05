#pragma once

#include <basis/seadTypes.h>
#include <math/seadVectorFwd.h>

#include "Library/Area/IUseAreaObj.h"
#include "Library/Audio/IUseAudioKeeper.h"

namespace al {
class AreaObjDirector;
class AudioKeeper;
class BgmPlayObj;
class PlayerHolder;

class BgmPlayEventController : public IUseAudioKeeper, public IUseAreaObj {
public:
    BgmPlayEventController(AudioKeeper* audioKeeper);

    void init3D(AreaObjDirector* areaObjDirector);
    void update();
    void setChangeAreaParams(const char* playName, bool isChangeArea, bool isForceStart);
    void tryStartBgmByChangeArea(bool isChangeArea, s32 startFrame, bool isForceStart);
    void setStopAreaParams();
    void tryStartBgmAfterStopAreaMove(bool isChangeArea, BgmPlayObj* obj, bool isForceStart);
    void stopBgmByStopArea(bool isForceStop);
    void updateWithoutEventStart();
    void finalize();
    void setPlayerHolder(const PlayerHolder* playerHolder);
    bool tryRegistBgmPlayObj(BgmPlayObj* obj);
    void reset();
    const char* getBgmPlayNameInCurPosition(bool isStopArea);
    const char* getBgmPlayNameInTargetPosition(const sead::Vector3f& position);
    void tryStartAndStopBgmInCurPosition(bool isStopArea, bool isForceStart);
    bool isInBgmStopArea(const char* playName);
    bool checkWhetherItCanStartBgm(const char* playName);
    void startBgm(bool isForceStart);

    AudioKeeper* getAudioKeeper() const override;
    AreaObjDirector* getAreaObjDirector() const override;

private:
    u8 _10[0x98];
};

static_assert(sizeof(BgmPlayEventController) == 0xa8);

}  // namespace al
