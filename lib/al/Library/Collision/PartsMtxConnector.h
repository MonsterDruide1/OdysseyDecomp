#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {

class MtxConnector {
public:
    MtxConnector();
    MtxConnector(const sead::Quatf&, const sead::Vector3f&);

    virtual bool isConnecting() const;
    virtual void clear();

    void init(const sead::Matrix34f* parentMtx, const sead::Matrix34f& mtx);
    void init(const sead::Matrix34f* parentMtx);
    void multVec(sead::Vector3f* outVec, const sead::Vector3f& vec) const;
    void multTrans(sead::Vector3f* outVec, const sead::Vector3f& vec) const;
    void multMtx(sead::Matrix34f* outMtx, const sead::Matrix34f& mtx) const;
    void multQT(sead::Quatf* outQuat, sead::Vector3f* outPos, sead::Vector3f* outScale) const;
    void multQT(sead::Quatf* outQuat, sead::Vector3f* outPos, sead::Vector3f* outScale,
                const sead::Quatf& baseQuat, const sead::Vector3f& baseTrans) const;
    void multQT(sead::Quatf* outQuat, sead::Vector3f* outPos, const sead::Quatf& baseQuat,
                const sead::Vector3f& baseTrans) const;
    const sead::Quatf& getBaseQuat() const;
    const sead::Vector3f& getBaseTrans() const;
    void setBaseQuatTrans(const sead::Quatf& quat, const sead::Vector3f& trans);
    void calcConnectInfo(sead::Vector3f* outPos, sead::Quatf* outQuat, sead::Vector3f* outScale,
                         const sead::Vector3f& trans, const sead::Vector3f& rotation) const;
    void calcMtxWithOffset(sead::Matrix34f* outMtx, const sead::Vector3f& trans,
                           const sead::Vector3f& rotation) const;
    bool tryGetParentTrans(sead::Vector3f* outTrans) const;

private:
    sead::Matrix34f mMtx = sead::Matrix34f::ident;
    const sead::Matrix34f* mParentMtx = nullptr;
    sead::Quatf mBaseQuat = sead::Quatf::unit;
    sead::Vector3f mBaseTrans = sead::Vector3f::zero;
};

static_assert(sizeof(MtxConnector) == 0x60);

}  // namespace al
