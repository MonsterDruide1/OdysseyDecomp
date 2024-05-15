#include "Library/Application/ApplicationMessageReceiver.h"

#include <nn/am.h>

namespace al {
ApplicationMessageReceiver::ApplicationMessageReceiver() {}

nn::oe::OperationMode ApplicationMessageReceiver::getOperationMode() const {
    nn::oe::OperationMode operationMode = nn::oe::GetOperationMode();

    switch (operationMode) {
    case nn::oe::OperationMode_Handheld:
        return nn::oe::OperationMode_Handheld;
    case nn::oe::OperationMode_Docked:
        return nn::oe::OperationMode_Docked;
    default:
        return nn::oe::OperationMode_Handheld;
    }
}

nn::oe::PerformanceMode ApplicationMessageReceiver::getPerformaceMode() const {
    nn::oe::PerformanceMode performanceMode = nn::oe::GetPerformanceMode();

    switch (performanceMode) {
    case nn::oe::PerformanceMode_Normal:
        return nn::oe::PerformanceMode_Normal;
    case nn::oe::PerformanceMode_Boost:
        return nn::oe::PerformanceMode_Boost;
    default:
        return nn::oe::PerformanceMode_Normal;
    }
}

void ApplicationMessageReceiver::init() {
    nn::oe::Initialize();

    mOperationMode = getOperationMode();
    mPerformanceMode = getPerformaceMode();

    nn::oe::SetResumeNotificationEnabled(true);
    nn::oe::SetOperationModeChangedNotificationEnabled(true);
    nn::oe::SetPerformanceModeChangedNotificationEnabled(true);
    nn::oe::SetFocusHandlingMode(nn::oe::FocusHandlingMode_AlwaysSuspend);
}

void ApplicationMessageReceiver::procMessage(u32 message) {
    switch (message) {
    case nn::am::AppletMessage_FocusStateChanged:
        nn::oe::GetCurrentFocusState();
        break;
    case nn::am::AppletMessage_OperationModeChanged:
        mIsUpdatedOperationMode = true;
        mOperationMode = getOperationMode();
        break;
    case nn::am::AppletMessage_PerformanceModeChanged:
        mIsUpdatedPerformanceMode = true;
        mPerformanceMode = getPerformaceMode();
        break;
    }
}

void ApplicationMessageReceiver::update() {
    mIsUpdatedOperationMode = false;
    mIsUpdatedPerformanceMode = false;
    u32 message;

    if (nn::oe::TryPopNotificationMessage(&message)) {
        procMessage(message);
    }
}
}  // namespace al
