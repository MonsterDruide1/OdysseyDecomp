#pragma once

#include "Library/Execute/IUseExecutor.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class ExecuteDirector;
class HitSensor;
class SensorHitGroup;

class HitSensorDirector : public IUseExecutor, public IUseHioNode {
public:
    HitSensorDirector(ExecuteDirector*);

    void execute() override;

    void initGroup(HitSensor*);
    void executeHitCheckInSameGroup(SensorHitGroup*) const;
    void executeHitCheckGroup(SensorHitGroup*, SensorHitGroup*) const;
    void executeHitCheck(HitSensor*, HitSensor*) const;

private:
    void* filler[8];
};
static_assert(sizeof(HitSensorDirector) == 0x48);

}  // namespace al
