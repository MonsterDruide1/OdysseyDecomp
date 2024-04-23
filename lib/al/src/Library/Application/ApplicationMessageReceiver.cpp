#include "Library/Application/ApplicationMessageReceiver.h"

namespace al {
ApplicationMessageReceiver::ApplicationMessageReceiver() {
    ukn[0] = false;
    ukn[1] = false;
    mOperationMode = nn::oe::OperationMode_Handheld;
    mPerformanceMode = nn::oe::PerformanceMode_Normal;
}

nn::oe::OperationMode ApplicationMessageReceiver::getOperationMode() const {
    nn::oe::OperationMode operationMode = nn::oe::GetOperationMode();

    if (operationMode != nn::oe::OperationMode_Handheld) {
        return (nn::oe::OperationMode)(operationMode == nn::oe::OperationMode_Docked);
    }

    return operationMode;
}

nn::oe::PerformanceMode ApplicationMessageReceiver::getPerformaceMode() const {
    nn::oe::PerformanceMode performanceMode = nn::oe::GetPerformanceMode();

    if (performanceMode != nn::oe::PerformanceMode_Normal) {
        return (nn::oe::PerformanceMode)(performanceMode == nn::oe::PerformanceMode_Boost);
    }

    return performanceMode;
}

void ApplicationMessageReceiver::init() {
    nn::oe::Initialize();

    mOperationMode = getOperationMode();
    mPerformanceMode = getPerformaceMode();

    nn::oe::SetResumeNotificationEnabled(true);
    nn::oe::SetOperationModeChangedNotificationEnabled(true);
    nn::oe::SetPerformanceModeChangedNotificationEnabled(true);
    nn::oe::SetFocusHandlingMode(3);  // enum?
}

void ApplicationMessageReceiver::procMessage(nn::oe::NotificationMessage message) {
    switch (message) {
    case nn::oe::NotificationMessage_Ukn:
        nn::oe::GetCurrentFocusState();
        break;
    case nn::oe::NotificationMessage_OperationModeChanged:
        ukn[0] = true;
        mOperationMode = getOperationMode();
        break;
    case nn::oe::NotificationMessage_PerformanceModeChanged:
        ukn[1] = true;
        mPerformanceMode = getPerformaceMode();
        break;
    }
}

void ApplicationMessageReceiver::update() {
    ukn[0] = false;
    ukn[1] = false;
    nn::oe::NotificationMessage message;

    if (nn::oe::TryPopNotificationMessage((u32*)&message)) {
        procMessage(message);
    }
}
}  // namespace al