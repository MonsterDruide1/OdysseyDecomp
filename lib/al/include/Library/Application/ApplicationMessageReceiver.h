#pragma once

#include <nn/oe.h>

namespace al {
class ApplicationMessageReceiver {
public:
    ApplicationMessageReceiver();

    nn::oe::OperationMode getOperationMode() const;
    nn::oe::PerformanceMode getPerformaceMode() const;

    void init();
    void procMessage(u32 message);  // enum ?
    void update();

private:
    bool ukn[4]{};
    nn::oe::OperationMode mOperationMode;
    nn::oe::PerformanceMode mPerformanceMode;
};

static_assert(sizeof(ApplicationMessageReceiver) == 0xc);
}  // namespace al
