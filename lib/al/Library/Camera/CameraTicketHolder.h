#pragma once

#include <basis/seadTypes.h>

namespace al {
class CameraTicket;
class PlacementId;

class CameraTicketHolder {
public:
    CameraTicketHolder(s32 maxTickets);

    void endInit();
    void registerTicket(CameraTicket* ticket);
    void registerDefaultTicket(CameraTicket* ticket);
    CameraTicket* tryFindEntranceTicket(const PlacementId* placementId, const char* suffix) const;

private:
    CameraTicket** mTickets;
    s32 mNumTickets = 0;
    s32 mMaxTickets;
    CameraTicket* mDefaultTicket = nullptr;
};

}  // namespace al
