#pragma once

#include "heap/seadDisposer.h"

namespace sead
{
class ProcessMeterBarBase;

// FIXME
class ProcessMeter
{
    SEAD_SINGLETON_DISPOSER(ProcessMeter)
public:
    void attachProcessMeterBar(ProcessMeterBarBase* bar);
    void detachProcessMeterBar(ProcessMeterBarBase* bar);
};
}  // namespace sead
