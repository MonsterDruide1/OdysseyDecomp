#include "Library/Camera/SpecialCameraHolder.h"

#include "Library/Base/StringUtil.h"
#include "Library/Camera/CameraTicket.h"
#include "Library/Camera/CameraTicketId.h"

namespace al {

SpecialCameraHolder::SpecialCameraHolder() {}

void SpecialCameraHolder::allocEntranceCameraBuffer(s32 maxEntries) {
    mMaxEntranceCameras = maxEntries;
    mEntranceCameras = new CameraTicket*[maxEntries];
    for (s32 i = 0; i < mMaxEntranceCameras; i++)
        mEntranceCameras[i] = nullptr;
}

void SpecialCameraHolder::registerEntranceCamera(CameraTicket* ticket) {
    mEntranceCameras[mNumEntranceCameras] = ticket;
    mNumEntranceCameras++;
}

CameraTicket* SpecialCameraHolder::findEntranceCamera(const char* suffix) const {
    for (s32 i = 0; i < mNumEntranceCameras; i++) {
        CameraTicket* ticket = mEntranceCameras[i];
        if (isEqualString(ticket->getTicketId()->getSuffix(), suffix))
            return ticket;
    }
    return nullptr;
}

}  // namespace al
