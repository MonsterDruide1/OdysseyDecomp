#include "Library/Placement/PlacementFunction.h"

#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Yaml/ByamlIter.h"

namespace al {
s32 getCountPlacementInfo(const PlacementInfo& rInfo) {
    return rInfo.getPlacementIter().getSize();
}

void getPlacementInfoByKey(PlacementInfo* pOutInfo, const PlacementInfo& rInfo, const char* pKey) {
    ByamlIter iterator;
    bool ret = rInfo.getPlacementIter().tryGetIterByKey(&iterator, pKey);

    if (ret)
        pOutInfo->set(iterator, rInfo.getZoneIter());
}

void getPlacementInfoByIndex(PlacementInfo* pOutInfo, const PlacementInfo& rInfo, s32 idx) {
    ByamlIter iterator;
    bool ret = rInfo.getPlacementIter().tryGetIterByIndex(&iterator, idx);

    if (ret)
        pOutInfo->set(iterator, rInfo.getZoneIter());
}

bool tryGetPlacementInfoByIndex(PlacementInfo* pOutInfo, const PlacementInfo& rInfo, s32 idx) {
    bool result;

    ByamlIter iterator;
    bool ret = rInfo.getPlacementIter().tryGetIterByIndex(&iterator, idx);

    if (ret) {
        pOutInfo->set(iterator, rInfo.getZoneIter());
        result = true;
    } else {
        result = false;
    }

    return result;
}

void getPlacementInfoAndKeyNameByIndex(PlacementInfo* pOutInfo, const char** pOutKeyName,
                                       const PlacementInfo& rInfo, s32 idx) {
    ByamlIter iterator;

    bool ret = rInfo.getPlacementIter().tryGetIterAndKeyNameByIndex(&iterator, pOutKeyName, idx);

    if (ret)
        pOutInfo->set(iterator, rInfo.getZoneIter());
}

bool tryGetPlacementInfoAndKeyNameByIndex(PlacementInfo* pOutInfo, const char** pOutKeyName,
                                          const PlacementInfo& rInfo, s32 idx) {
    bool result;

    ByamlIter iterator;
    bool ret = rInfo.getPlacementIter().tryGetIterAndKeyNameByIndex(&iterator, pOutKeyName, idx);

    if (ret) {
        pOutInfo->set(iterator, rInfo.getZoneIter());
        result = true;
    } else {
        result = false;
    }

    return result;
}

PlacementId* createPlacementId(const ActorInitInfo& rInfo) {
    const PlacementInfo& info = rInfo.getPlacementInfo();
    PlacementId* id = new PlacementId();
    id->init(info);
    return id;
}

PlacementId* createPlacementId(const PlacementInfo& rInfo) {
    PlacementId* id = new PlacementId();
    id->init(rInfo);
    return id;
}

bool tryGetPlacementId(PlacementId* pOut, const ActorInitInfo& rInfo) {
    return pOut->init(rInfo.getPlacementInfo());
}

bool tryGetPlacementId(PlacementId* pOut, const PlacementInfo& rInfo) {
    return pOut->init(rInfo);
}

void getPlacementId(PlacementId* pOut, const ActorInitInfo& rInfo) {
    pOut->init(rInfo.getPlacementInfo());
}

void getPlacementId(PlacementId* pOut, const PlacementInfo& rInfo) {
    pOut->init(rInfo);
}

bool isEqualPlacementId(const PlacementId& rLhs, const PlacementId& rRhs) {
    return rLhs.isEqual(rRhs);
}

bool isEqualPlacementId(const PlacementInfo& rLhs, const PlacementInfo& rRhs) {
    bool res;
    PlacementId id_lhs;

    if (id_lhs.init(rLhs)) {
        PlacementId id_rhs;

        if (id_rhs.init(rRhs))
            res = id_lhs.isEqual(id_rhs);
        else
            res = false;

    } else {
        res = false;
    }

    return res;
}
}  // namespace al
