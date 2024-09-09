#include "Library/Camera/CameraTicketHolder.h"

#include "Library/Camera/CameraPoser.h"
#include "Library/Camera/CameraTicket.h"
#include "Library/Camera/CameraTicketId.h"

namespace al {

CameraTicketHolder::CameraTicketHolder(s32 maxTickets) : mMaxTickets(maxTickets) {
    mTickets = new CameraTicket*[maxTickets];
    for (s32 i = 0; i < mMaxTickets; i++)
        mTickets[i] = nullptr;
}

void CameraTicketHolder::endInit() {
    for (s32 i = 0; i < mNumTickets; i++)
        mTickets[i]->getPoser()->endInit();

    if (mDefaultTicket)
        mDefaultTicket->getPoser()->endInit();
}

void CameraTicketHolder::registerTicket(CameraTicket* ticket) {
    if (ticket->getPriority() == CameraTicket::Priority_Default) {
        registerDefaultTicket(ticket);
        return;
    }

    mTickets[mNumTickets] = ticket;
    mNumTickets++;
}

void CameraTicketHolder::registerDefaultTicket(CameraTicket* ticket) {
    mDefaultTicket = ticket;
}

CameraTicket* CameraTicketHolder::tryFindEntranceTicket(const PlacementId* placementId,
                                                        const char* suffix) const {
    CameraTicketId searchTicketId = {placementId, suffix};

    for (s32 i = 0; i < mNumTickets; i++)
        if (mTickets[i]->getPriority() == CameraTicket::Priority_Entrance &&
            mTickets[i]->getTicketId()->isEqual(searchTicketId))
            return mTickets[i];

    return nullptr;
}

}  // namespace al
