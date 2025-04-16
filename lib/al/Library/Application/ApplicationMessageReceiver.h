#pragma once

#include <nn/oe.h>

namespace al {
class ApplicationMessageReceiver {
public:
    ApplicationMessageReceiver();

    nn::oe::OperationMode getOperationMode() const;
    nn::oe::PerformanceMode getPerformaceMode() const;

    void init();
    void procMessage(u32 message);
    void update();

private:
    bool mIsUpdatedOperationMode = false;
    bool mIsUpdatedPerformanceMode = false;
    nn::oe::OperationMode mOperationMode = nn::oe::OperationMode_Handheld;
    nn::oe::PerformanceMode mPerformanceMode = nn::oe::PerformanceMode_Normal;
};

static_assert(sizeof(ApplicationMessageReceiver) == 0xc);
}  // namespace al
