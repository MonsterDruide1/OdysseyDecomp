#pragma once

#include <nn/oe.h>

namespace nn::oe {
enum NotificationMessage {
    NotificationMessage_FocusStateChanged = 0xf,  // not sure
    NotificationMessage_OperationModeChanged = 0x1e,
    NotificationMessage_PerformanceModeChanged = 0x1f
};  // should go into oe.h
}  // namespace nn::oe

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
    bool ukn[4]{};
    nn::oe::OperationMode mOperationMode;
    nn::oe::PerformanceMode mPerformanceMode;
};

static_assert(sizeof(ApplicationMessageReceiver) == 0xc);
}  // namespace al
