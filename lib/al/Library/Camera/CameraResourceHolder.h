#pragma once

#include <prim/seadSafeString.h>

namespace al {
class Resource;
class ByamlIter;
class CameraTicketId;
class PlacementId;

class CameraResourceHolder {
public:
    struct Entry {
        ByamlIter* cameraParam = nullptr;
        ByamlIter* interpoleParam = nullptr;
        sead::FixedSafeString<128> stageName = {""};
    };

    enum ParamType : s32 {
        DefaultTickets = 0,
        StartTickets = 2,
        Tickets  // any other value
    };

    CameraResourceHolder(const char* stageName, s32 maxResources);

    bool tryInitCameraResource(const Resource* resource, s32 unused);
    bool tryFindParamResource(ByamlIter* ticket, const CameraTicketId* ticketId,
                              s32 paramType) const;
    bool tryFindCameraParamList(ByamlIter* paramList, const PlacementId* placementId,
                                const char* paramName) const;
    s32 calcEntranceCameraParamNum() const;
    bool tryFindCameraParamList(ByamlIter* paramList, const char* stageName,
                                const char* paramName) const;
    void getEntranceCameraParamResource(ByamlIter* ticket, s32 index) const;
    Entry* findCameraResource(const char* stageName) const;
    Entry* tryFindCameraResource(const char* stageName) const;
    Entry* tryFindCameraResource(const PlacementId* placementId) const;

private:
    const char* mStageName;
    s32 mMaxEntries;
    s32 mNumEntries = 0;
    Entry** mEntries;
};

}  // namespace al
