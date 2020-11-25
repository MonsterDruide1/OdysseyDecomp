#pragma once

#include "sead/seadMatrix.h"
#include "sead/seadQuat.h"
#include "sead/seadVector.h"

namespace al
{
    class ActorPoseKeeperBase
    {
    public:
        ActorPoseKeeperBase();

        virtual const sead::Vector3<float>& getRotate() const;
        virtual const sead::Vector3<float>& getScale() const;
        virtual const sead::Vector3<float>& getVelocity() const;
        virtual const sead::Vector3<float>& getFront() const;
        virtual const sead::Vector3<float>& getUp() const;
        virtual const sead::Quat<float>& getQuat() const;
        virtual const sead::Vector3<float>& getGravity() const;
        virtual const sead::Matrix34<float>& getMtx() const;
        virtual sead::Vector3<float>* getRotatePtr();
        virtual sead::Vector3<float>* getScalePtr();
        virtual sead::Vector3<float>* getVelocityPtr();
        virtual sead::Vector3<float>* getFrontPtr();
        virtual sead::Vector3<float>* getUpPtr();
        virtual sead::Quat<float>* getQuatPtr();
        virtual sead::Vector3<float>* getGravityPtr();
        virtual sead::Matrix34<float>* getMtxPtr();
        virtual void updatePoseTrans(const sead::Vector3<float> &) = 0;
        virtual void updatePoseRotate(const sead::Vector3<float> &) = 0;
        virtual void updatePoseQuat(const sead::Quat<float> &) = 0;
        virtual void updatePoseMtx(const sead::Matrix34<float> *) = 0;
        virtual void copyPose(const al::ActorPoseKeeperBase *);
        virtual void calcBaseMtx(sead::Matrix34<float> *) = 0;

        unsigned int _8 = 0;
        unsigned int _C = 0;
        unsigned int _10 = 0;

        static const sead::Vector3<float> sDefaultVelocity;
    };
};