#include "al/actor/pose/ActorPoseKeeperBase.h"

namespace al
{
    const sead::Vector3<float>& ActorPoseKeeperBase::getRotate() const
    {
        return sead::Vector3<float>::zero;
    }

    const sead::Vector3<float>& ActorPoseKeeperBase::getScale() const
    {
        return sead::Vector3<float>::ones;
    }

    const sead::Vector3<float>& ActorPoseKeeperBase::getVelocity() const
    {
        return sead::Vector3<float>::zero;
    }

    const sead::Vector3<float>& ActorPoseKeeperBase::getFront() const
    {
        return sead::Vector3<float>::ez;
    }

    const sead::Vector3<float>& ActorPoseKeeperBase::getUp() const
    {
        return sead::Vector3<float>::ey;
    }

    const sead::Quat<float>& ActorPoseKeeperBase::getQuat() const
    {
        return sead::Quat<float>::unit;
    }

    const sead::Vector3<float>& ActorPoseKeeperBase::getGravity() const
    {
        return sDefaultVelocity;
    }

    const sead::Matrix34<float>& ActorPoseKeeperBase::getMtx() const
    {
        return sead::Matrix34<float>::ident;
    }

    sead::Vector3<float>* ActorPoseKeeperBase::getRotatePtr()
    {
        return nullptr;
    }

    sead::Vector3<float>* ActorPoseKeeperBase::getScalePtr()
    {
        return nullptr;
    }

    sead::Vector3<float>* ActorPoseKeeperBase::getVelocityPtr()
    {
        return nullptr;
    }

    sead::Vector3<float>* ActorPoseKeeperBase::getFrontPtr()
    {
        return nullptr;
    }

    sead::Vector3<float>* ActorPoseKeeperBase::getUpPtr()
    {
        return nullptr;
    }

    sead::Quat<float>* ActorPoseKeeperBase::getQuatPtr()
    {
        return nullptr;
    }

    sead::Vector3<float>* ActorPoseKeeperBase::getGravityPtr()
    {
        return nullptr;
    }

    sead::Matrix34<float>* ActorPoseKeeperBase::getMtxPtr()
    {
        return nullptr;
    }
};