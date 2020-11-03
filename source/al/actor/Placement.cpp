#include "al/actor/Placement.h"

namespace al
{
    PlacementInfo::PlacementInfo()
        : _0(), mZoneIter()
    { }

    void PlacementInfo::set(const al::ByamlIter &r_0, const al::ByamlIter &rZoneIter)
    {
        _0.mDataOffset = r_0.mDataOffset;
        _0.mData = r_0.mData;        

        mZoneIter.mDataOffset = rZoneIter.mDataOffset;
        mZoneIter.mData = rZoneIter.mData;
    }

    PlacementId::PlacementId()
        : mID(nullptr), mCommonID(nullptr), _0(nullptr), mUnitConfigName(nullptr)
    { }

    PlacementId::PlacementId(const char *p_0, const char *pUnitConfig, const char *pID)
        : _0(p_0), mUnitConfigName(pUnitConfig), mID(pID), mCommonID(nullptr)
    { }

    void PlacementId::init(const al::PlacementInfo &rInfo)
    {
        _0 = nullptr, mID = nullptr, mUnitConfigName = nullptr, mCommonID = nullptr;

        rInfo._0.tryGetStringByKey(&mCommonID, "CommonId");
        rInfo.mZoneIter.tryGetStringByKey(&mUnitConfigName, "UnitConfigName");
        rInfo.mZoneIter.tryGetStringByKey(&mID, "Id");
        rInfo._0.tryGetStringByKey(&_0, "Id");
    }

    bool PlacementId::isValid() const
    {
        bool ret;

        if (mCommonID)
        {
            ret = true;
        }
        else
        {
            ret = _0 != nullptr;
        }
        
        return ret;
    }
};