#include "al/util/PlacementUtil.h"

namespace al {
int getCountPlacementInfo(const al::PlacementInfo& rInfo) {
    return rInfo._0.getSize();
}

void getPlacementInfoByKey(al::PlacementInfo* pOutInfo, const al::PlacementInfo& rInfo,
                           const char* pKey) {
    al::ByamlIter iterator;
    bool ret = rInfo._0.tryGetIterByKey(&iterator, pKey);

    if (ret) {
        pOutInfo->set(iterator, rInfo.mZoneIter);
    }
}

void getPlacementInfoByIndex(al::PlacementInfo* pOutInfo, const al::PlacementInfo& rInfo, int idx) {
    al::ByamlIter iterator;
    bool ret = rInfo._0.tryGetIterByIndex(&iterator, idx);

    if (ret) {
        pOutInfo->set(iterator, rInfo.mZoneIter);
    }
}

bool tryGetPlacementInfoByIndex(al::PlacementInfo* pOutInfo, const al::PlacementInfo& rInfo,
                                int idx) {
    bool result;

    al::ByamlIter iterator;
    bool ret = rInfo._0.tryGetIterByIndex(&iterator, idx);

    if (ret) {
        pOutInfo->set(iterator, rInfo.mZoneIter);
        result = true;
    } else {
        result = false;
    }

    return result;
}

void getPlacementInfoAndKeyNameByIndex(al::PlacementInfo* pOutInfo, const char** pOutKeyName,
                                       const al::PlacementInfo& rInfo, int idx) {
    al::ByamlIter iterator;

    bool ret = rInfo._0.tryGetIterAndKeyNameByIndex(&iterator, pOutKeyName, idx);

    if (ret) {
        pOutInfo->set(iterator, rInfo.mZoneIter);
    }
}

bool tryGetPlacementInfoAndKeyNameByIndex(al::PlacementInfo* pOutInfo, const char** pOutKeyName,
                                          const al::PlacementInfo& rInfo, int idx) {
    bool result;

    al::ByamlIter iterator;
    bool ret = rInfo._0.tryGetIterAndKeyNameByIndex(&iterator, pOutKeyName, idx);

    if (ret) {
        pOutInfo->set(iterator, rInfo.mZoneIter);
        result = true;
    } else {
        result = false;
    }

    return result;
}

al::PlacementId* createPlacementId(const al::ActorInitInfo& rInfo) {
    const al::PlacementInfo& info = rInfo.mPlacementInfo;
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
    return pOut->init(rInfo.mPlacementInfo);
}

bool tryGetPlacementId(al::PlacementId* pOut, const al::PlacementInfo& rInfo) {
    return pOut->init(rInfo);
}

void getPlacementId(al::PlacementId* pOut, const al::ActorInitInfo& rInfo) {
    pOut->init(rInfo.mPlacementInfo);
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