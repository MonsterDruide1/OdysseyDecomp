#pragma once

#include <basis/seadTypes.h>

class FriendsProfileDataHolder;

namespace nn::nex {
template <typename K, typename V>
class qMap;
struct DataStoreRatingInfo;
}  // namespace nn::nex

namespace TimeBalloon {
class BalloonData;
struct BalloonDataMetaBinary;
}  // namespace TimeBalloon

class TimeBalloonBalloonDataHolder {
public:
    TimeBalloonBalloonDataHolder(s32 dataMax);

    void reset();
    void setData(const TimeBalloon::BalloonDataMetaBinary* metaBinary, u64 dataId, u64 ownerId,
                 u32 worldTypeId,
                 const nn::nex::qMap<s8, nn::nex::DataStoreRatingInfo>& ratingInfoMap,
                 FriendsProfileDataHolder* profileDataHolder, bool isGotBalloon);
    bool trySetData(const TimeBalloon::BalloonDataMetaBinary* metaBinary, u64 dataId, u64 ownerId,
                    u32 worldTypeId,
                    const nn::nex::qMap<s8, nn::nex::DataStoreRatingInfo>& ratingInfoMap,
                    FriendsProfileDataHolder* profileDataHolder, bool isGotBalloon);
    void clearData(s32 index);
    const TimeBalloon::BalloonData& getData(s32 index) const;
    const TimeBalloon::BalloonData* tryGetData(s32 index) const;
    const TimeBalloon::BalloonData* tryGetData(u64 dataId) const;
    const TimeBalloon::BalloonData* tryGetDataByWorldTypeId(u32 worldTypeId) const;
    bool tryClearData(u64 dataId);

    s32 getCurrentCount() const { return mCurrentCount; }

private:
    u8 _0[0x10];
    s32 mCurrentCount = 0;
};
