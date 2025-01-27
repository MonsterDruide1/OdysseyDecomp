#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <math/seadVector.h>

#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
class IUseSceneObjHolder;
}  // namespace al

class CoinCollect;
class CoinCollect2D;
class CoinCollectHintObj;

class CoinCollectHolder : public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_CoinCollectHolder;

    CoinCollectHolder();

    const char* getSceneObjName() const override;

    void registerCoinCollect(CoinCollect* coin);
    void registerCoinCollect2D(CoinCollect2D* coin);
    void registerHintObj(CoinCollectHintObj* hintObj);

    CoinCollect* tryFindAliveCoinCollect(const sead::Vector3f& playerPos, bool checkAreaOb) const;
    CoinCollect* tryFindAliveCoinCollect(const sead::Vector3f& playerPos, f32 minRange,
                                         f32 maxRange, bool checkAreaObj) const;
    CoinCollect* tryFindDeadButHintEnableCoinCollect() const;
    CoinCollect2D* tryFindAliveCoinCollect2D(const sead::Vector3f& playerPos,
                                             bool checkAreaOb) const;
    CoinCollect2D* tryFindAliveCoinCollect2D(const sead::Vector3f& playerPos, f32 minRange,
                                             f32 maxRange, bool checkAreaObj) const;
    bool tryFindExStageHintObjTrans(sead::Vector3f* outTrans, const char* stageName);

private:
    sead::PtrArray<CoinCollect> mCollect;
    sead::PtrArray<CoinCollect2D> mCollect2D;
    sead::PtrArray<CoinCollectHintObj> mCollectHintObj;
};

namespace rs {
CoinCollectHolder* createCoinCollectHolder(const al::IUseSceneObjHolder* objHolder);
}  // namespace rs
