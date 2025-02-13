#pragma once

#include <basis/seadTypes.h>

namespace al {
class CameraPoser;
class CameraTicketId;

class CameraTicket {
public:
    enum Priority {
        Priority_Default = 0,
        Priority_Entrance = 2,
        Priority_BossField = 3,
        Priority_Capture = 4,
        Priority_Object = 5,
        Priority_ForceArea = 6,
        // Priority_EntranceAll = 7,
        Priority_SafetyPointRecovery = 8,
        Priority_Player = 9,
        Priority_DemoTalk = 10,
        Priority_Demo = 11,
        Priority_Demo2 = 12,
    };

    CameraTicket(CameraPoser* poser, const CameraTicketId* ticketId, s32 priority);
    void setPriority(s32 priority);

    CameraPoser* getPoser() const { return mPoser; }

    const CameraTicketId* getTicketId() const { return mTicketId; }

    s32 getPriority() const { return mPriority; }

    bool getActive() const { return mIsActiveCamera; }

    void setActive(bool state) { mIsActiveCamera = state; }

private:
    CameraPoser* mPoser;
    const CameraTicketId* mTicketId;
    s32 mPriority;
    bool mIsActiveCamera = false;
};

}  // namespace al
