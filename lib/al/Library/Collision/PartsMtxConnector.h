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

    void init(const sead::Matrix34f*, const sead::Matrix34f&) const;
    void init(const sead::Matrix34f*) const;
    void multVec(sead::Vector3f*, const sead::Vector3f&) const;
    void multTrans(sead::Vector3f*, const sead::Vector3f&) const;
    void multMtx(sead::Matrix34f*, const sead::Matrix34f&) const;
    void multQT(sead::Quatf*, sead::Vector3f*, sead::Vector3f*) const;
    void multQT(sead::Quatf*, sead::Vector3f*, sead::Vector3f*, const sead::Quatf&,
                const sead::Vector3f&) const;
    void multQT(sead::Quatf*, sead::Vector3f*, const sead::Quatf&, const sead::Vector3f&) const;
    const sead::Quatf& getBaseQuat() const;
    const sead::Vector3f& getBaseTrans() const;
    void setBaseQuatTrans(const sead::Quatf&, const sead::Vector3f&);
    void calcConnectInfo(sead::Vector3f*, sead::Quatf*, sead::Vector3f*, const sead::Vector3f&,
                         const sead::Vector3f&) const;
    void calcMtxWithOffset(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&) const;
    bool tryGetParentTrans(sead::Vector3f*) const;

private:
    void* _0[0x58 / 8];
};

static_assert(sizeof(MtxConnector) == 0x60);

}  // namespace al
