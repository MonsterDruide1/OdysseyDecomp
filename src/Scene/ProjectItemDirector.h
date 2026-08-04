#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/HostIO/HioNode.h"
#include "Library/Scene/ItemDirectorBase.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class LiveActor;
}  // namespace al

class GameDataHolder;

class ProjectItemDirector : public al::HioNode, public al::ItemDirectorBase {
public:
    ProjectItemDirector();
    ProjectItemDirector(const al::ActorInitInfo&);

    void appearItem(const char*, const sead::Vector3f&, const sead::Quatf&,
                    const al::HitSensor*) override;

    void tryAppearPlayerDeadCoin(const al::LiveActor*, const sead::Vector3f&);
    void acquireItem(const al::LiveActor*, al::HitSensor*, const char*) const override;
    void declareItem(const char*, const al::ActorInitInfo&) override;
    void endInit() override;
    void tryAddDemoCountUpCoin();
    void initPlayerDeadCoin(GameDataHolder*, const al::ActorInitInfo&);
    void setCoinTransForDeadPlayer(GameDataHolder*);
    void tryHidePlayerDeadCoinForDemo();
    void tryShowPlayerDeadCoinForDemo();

private:
};
