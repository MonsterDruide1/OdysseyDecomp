#pragma once

namespace al {
class PlacementId;
class ByamlIter;

class CameraTicketId {
public:
    CameraTicketId(const PlacementId* placementId, const char* suffix);

    bool isEqual(const CameraTicketId& other) const;
    static bool isEqual(const CameraTicketId& ticket1, const CameraTicketId& ticket2);
    bool isEqual(const ByamlIter& iter) const;

    const char* tryGetObjId() const;
    const char* getObjId() const;

    const PlacementId* getPlacementId() const { return mPlacementId; }

    const char* getSuffix() const { return mSuffix; }

private:
    const PlacementId* mPlacementId;
    const char* mSuffix;
};

}  // namespace al
