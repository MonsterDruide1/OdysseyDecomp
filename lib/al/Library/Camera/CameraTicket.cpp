#include "Library/Camera/CameraTicket.h"

namespace al {

CameraTicket::CameraTicket(CameraPoser* poser, const CameraTicketId* ticketId, s32 priority)
    : mPoser(poser), mTicketId(ticketId), mPriority(priority) {}

void CameraTicket::setPriority(s32 priority) {
    mPriority = priority;
}

}  // namespace al
