#include "Library/Camera/CameraResourceHolder.h"

#include <prim/seadSafeString.h>

#include "Library/Base/Macros.h"
#include "Library/Base/StringUtil.h"
#include "Library/Camera/CameraTicketId.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Resource/Resource.h"
#include "Library/Yaml/ByamlIter.h"

namespace al {

CameraResourceHolder::CameraResourceHolder(const char* stageName, s32 maxResources)
    : mStageName(stageName), mMaxEntries(maxResources) {
    mEntries = new Entry*[maxResources];
    for (s32 i = 0; i < mMaxEntries; i++)
        mEntries[i] = nullptr;
}

static void getStageName(StringTmp<128>* stageName, const char* archiveName) {
    StringTmp<256> safeArchiveName;
    safeArchiveName.format("%s", archiveName);
    if (safeArchiveName.endsWith("Map"))
        stageName->copy(safeArchiveName, safeArchiveName.calcLength() - 3);
}

bool CameraResourceHolder::tryInitCameraResource(const Resource* resource, s32 unused) {
    StringTmp<128> stageName = "";
    getStageName(&stageName, resource->getArchiveName());

    for (s32 i = 0; i < mNumEntries; i++)
        if (isEqualString(stageName.cstr(), mEntries[i]->stageName))
            return false;

    Entry* entry = new Entry;

    if (resource->isExistFile(StringTmp<64>{"%s.byml", "CameraParam"}))
        entry->cameraParam = new ByamlIter(resource->getByml("CameraParam"));

    if (resource->isExistFile(StringTmp<64>{"%s.byml", "InterpoleParam"}))
        entry->interpoleParam = new ByamlIter(resource->getByml("InterpoleParam"));

    entry->stageName = stageName;

    mEntries[mNumEntries] = entry;
    mNumEntries++;
    return true;
}

bool CameraResourceHolder::tryFindParamResource(ByamlIter* ticket, const CameraTicketId* ticketId,
                                                s32 paramType) const {
    ByamlIter paramList;
    const PlacementId* placementId = ticketId->getPlacementId();
    const char* paramName;
    if (paramType == 2)
        paramName = "StartTickets";
    else if (paramType == 0)
        paramName = "DefaultTickets";
    else
        paramName = "Tickets";

    if (!tryFindCameraParamList(&paramList, placementId, paramName))
        return false;

    for (s32 i = 0; i < paramList.getSize(); i++) {
        if (paramList.tryGetIterByIndex(ticket, i)) {
            ByamlIter id;
            ticket->tryGetIterByKey(&id, "Id");
            if (ticketId->isEqual(id))
                return true;
        }
    }
    return false;
}

bool CameraResourceHolder::tryFindCameraParamList(ByamlIter* paramList,
                                                  const PlacementId* placementId,
                                                  const char* paramName) const {
    if (placementId && placementId->getUnitConfigName())
        return tryFindCameraParamList(paramList, placementId->getUnitConfigName(), paramName);
    else
        return tryFindCameraParamList(paramList, mStageName, paramName);
}

s32 CameraResourceHolder::calcEntranceCameraParamNum() const {
    ByamlIter startTickets;
    if (!tryFindCameraParamList(&startTickets, mStageName, "StartTickets"))
        return 0;
    return startTickets.getSize();
}

bool CameraResourceHolder::tryFindCameraParamList(ByamlIter* paramList, const char* stageName,
                                                  const char* paramName) const {
    CameraResourceHolder::Entry* entry = findCameraResource(stageName);
    if (!entry || !entry->cameraParam)
        return false;
    return entry->cameraParam->tryGetIterByKey(paramList, paramName);
}

void CameraResourceHolder::getEntranceCameraParamResource(ByamlIter* ticket, s32 index) const {
    ByamlIter startTickets;
    tryFindCameraParamList(&startTickets, mStageName, "StartTickets");
    startTickets.tryGetIterByIndex(ticket, index);
}

CameraResourceHolder::Entry* CameraResourceHolder::findCameraResource(const char* stageName) const {
    for (s32 i = 0; i < mNumEntries; i++)
        if (isEqualString(stageName, mEntries[i]->stageName))
            return mEntries[i];

    return nullptr;
}

CameraResourceHolder::Entry*
CameraResourceHolder::tryFindCameraResource(const char* stageName) const {
    return findCameraResource(stageName);
}

CameraResourceHolder::Entry*
CameraResourceHolder::tryFindCameraResource(const PlacementId* placementId) const {
    if (placementId) {
        const char* name = placementId->getUnitConfigName();
        if (!name)
            name = mStageName;
        return findCameraResource(name);
    } else {
        return findCameraResource(mStageName);
    }
}

}  // namespace al
