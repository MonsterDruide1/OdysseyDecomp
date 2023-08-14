#include "Library/Placement/PlacementFunction.h"

#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Yaml/ByamlIter.h"

namespace al {
s32 getCountPlacementInfo(const al::PlacementInfo& rInfo) {
    return rInfo.getPlacementIter().getSize();
}

void getPlacementInfoByKey(al::PlacementInfo* pOutInfo, const al::PlacementInfo& rInfo,
                           const char* pKey) {
    al::ByamlIter iterator;
    bool ret = rInfo.getPlacementIter().tryGetIterByKey(&iterator, pKey);

    if (ret) {
        pOutInfo->set(iterator, rInfo.getZoneIter());
    }
}

void getPlacementInfoByIndex(al::PlacementInfo* pOutInfo, const al::PlacementInfo& rInfo, s32 idx) {
    al::ByamlIter iterator;
    bool ret = rInfo.getPlacementIter().tryGetIterByIndex(&iterator, idx);

    if (ret) {
        pOutInfo->set(iterator, rInfo.getZoneIter());
    }
}

bool tryGetPlacementInfoByIndex(al::PlacementInfo* pOutInfo, const al::PlacementInfo& rInfo,
                                s32 idx) {
    bool result;

    al::ByamlIter iterator;
    bool ret = rInfo.getPlacementIter().tryGetIterByIndex(&iterator, idx);

    if (ret) {
        pOutInfo->set(iterator, rInfo.getZoneIter());
        result = true;
    } else {
        result = false;
    }

    return result;
}

void getPlacementInfoAndKeyNameByIndex(al::PlacementInfo* pOutInfo, const char** pOutKeyName,
                                       const al::PlacementInfo& rInfo, s32 idx) {
    al::ByamlIter iterator;

    bool ret = rInfo.getPlacementIter().tryGetIterAndKeyNameByIndex(&iterator, pOutKeyName, idx);

    if (ret) {
        pOutInfo->set(iterator, rInfo.getZoneIter());
    }
}

bool tryGetPlacementInfoAndKeyNameByIndex(al::PlacementInfo* pOutInfo, const char** pOutKeyName,
                                          const al::PlacementInfo& rInfo, s32 idx) {
    bool result;

    al::ByamlIter iterator;
    bool ret = rInfo.getPlacementIter().tryGetIterAndKeyNameByIndex(&iterator, pOutKeyName, idx);

    if (ret) {
        pOutInfo->set(iterator, rInfo.getZoneIter());
        result = true;
    } else {
        result = false;
    }

    return result;
}

al::PlacementId* createPlacementId(const al::ActorInitInfo& rInfo) {
    const al::PlacementInfo& info = rInfo.getPlacementInfo();
    al::PlacementId* id = new al::PlacementId();
    id->init(info);
    return id;
}

al::PlacementId* createPlacementId(const al::PlacementInfo& rInfo) {
    al::PlacementId* id = new al::PlacementId();
    id->init(rInfo);
    return id;
}

bool tryGetPlacementId(al::PlacementId* pOut, const al::ActorInitInfo& rInfo) {
    return pOut->init(rInfo.getPlacementInfo());
}

bool tryGetPlacementId(al::PlacementId* pOut, const al::PlacementInfo& rInfo) {
    return pOut->init(rInfo);
}

void getPlacementId(al::PlacementId* pOut, const al::ActorInitInfo& rInfo) {
    pOut->init(rInfo.getPlacementInfo());
}

void getPlacementId(al::PlacementId* pOut, const al::PlacementInfo& rInfo) {
    pOut->init(rInfo);
}

bool isEqualPlacementId(const al::PlacementId& rLhs, const al::PlacementId& rRhs) {
    return rLhs.isEqual(rRhs);
}

bool isEqualPlacementId(const al::PlacementInfo& rLhs, const al::PlacementInfo& rRhs) {
    bool res;
    al::PlacementId id_lhs;

    if (id_lhs.init(rLhs)) {
        al::PlacementId id_rhs;

        if (id_rhs.init(rRhs)) {
            res = id_lhs.isEqual(id_rhs);
        } else {
            res = false;
        }

    } else {
        res = false;
    }

    return res;
}
}  // namespace al
