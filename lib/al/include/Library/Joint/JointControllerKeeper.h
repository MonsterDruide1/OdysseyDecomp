#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
class JointControllerBase;
class JointDirectionInfo;
class JointAimInfo;
class JointTranslateShaker;
class JointMasher;
class JointLookAtController;

void initJointControllerKeeper(const LiveActor*, s32);
void isExistJointControllerKeeper(const LiveActor*);
void registerJointController(const LiveActor*, JointControllerBase*);
void registerJointController(const LiveActor*, JointControllerBase*, const char*);
void initJointLocalRotator(const LiveActor*, sead::Vector3<f32>*, const char*);
void initJointLocalXRotator(const LiveActor*, f32 const*, const char*);
void initJointLocalYRotator(const LiveActor*, f32 const*, const char*);
void initJointLocalZRotator(const LiveActor*, f32 const*, const char*);
void initJointLocalAxisRotator(const LiveActor*, const sead::Vector3f&, f32 const*, const char*,
                               bool);
void initJointLocalMinusXRotator(const LiveActor*, f32 const*, const char*);
void initJointLocalMinusYRotator(const LiveActor*, f32 const*, const char*);
void initJointLocalMinusZRotator(const LiveActor*, f32 const*, const char*);
void initJointGlobalXRotator(const LiveActor*, f32*, const char*);
void initJointGlobalAxisRotator(const LiveActor*, const sead::Vector3f&, f32*, const char*);
void initJointGlobalYRotator(const LiveActor*, f32*, const char*);
void initJointGlobalZRotator(const LiveActor*, f32*, const char*);
void initJointGlobalMinusXRotator(const LiveActor*, f32*, const char*);
void initJointGlobalMinusYRotator(const LiveActor*, f32*, const char*);
void initJointGlobalMinusZRotator(const LiveActor*, f32*, const char*);
void initJointLocalTransControllerX(const LiveActor*, f32 const*, const char*);
void initJointLocalTransControllerY(const LiveActor*, f32 const*, const char*);
void initJointLocalTransControllerZ(const LiveActor*, f32 const*, const char*);
void initJointLocalTransController(const LiveActor*, const sead::Vector3f*, const char*);
void initJointLocalScaleControllerX(const LiveActor*, f32 const*, const char*);
void initJointLocalScaleControllerY(const LiveActor*, f32 const*, const char*);
void initJointLocalScaleControllerZ(const LiveActor*, f32 const*, const char*);
void initJointLocalScaleController(const LiveActor*, const sead::Vector3f*, const char*);
void initJointLocalMtxController(const LiveActor*, const sead::Matrix34f*, const char*);
void initJointGlobalMtxController(const LiveActor*, const sead::Matrix34f*, const char*);
void initJointGlobalQuatController(const LiveActor*, sead::Quat<f32> const*, const char*);
void initJointGlobalQuatTransController(const LiveActor*, sead::Quat<f32> const*,
                                        const sead::Vector3f*, const char*);
void initJointPostQuatController(const LiveActor*, sead::Quat<f32> const*, const char*);
void initJointLocalDirController(const LiveActor*, JointDirectionInfo const*, const char*);
void initJointAimController(const LiveActor*, JointAimInfo const*, const char*);
void initJointTranslateShaker(const LiveActor*, s32);
void appendJointTranslateShakerX(JointTranslateShaker*, const char*);
void appendJointTranslateShakerY(JointTranslateShaker*, const char*);
void appendJointTranslateShakerZ(JointTranslateShaker*, const char*);
void initJointMasher(const LiveActor*, bool const*, s32);
void appendMashJoint(JointMasher*, const char*, f32);
void initJointRumbler(const LiveActor*, const char*, f32, f32, u32, s32);
void initJointLocalQuatRotator(const LiveActor*, const char*, const sead::Quatf*);
void initJointLookAtController(const LiveActor*, s32);
void appendJointLookAtController(JointLookAtController*, const LiveActor*, const char*, f32,
                                 const sead::Vector2f&, const sead::Vector2f&,
                                 const sead::Vector3f&, const sead::Vector3f&);
void appendJointLookAtControllerNoJudge(JointLookAtController*, const LiveActor*, const char*, f32,
                                        const sead::Vector2f&, const sead::Vector2f&,
                                        const sead::Vector3f&, const sead::Vector3f&);
void appendJointLookAtControllerNoJudgeNoOverLimitYaw(JointLookAtController*, const LiveActor*,
                                                      const char*, f32, const sead::Vector2f&,
                                                      const sead::Vector2f&, const sead::Vector3f&,
                                                      const sead::Vector3f&);
void initJointGroundSmoothController(const LiveActor*, const char*);
void initJointPosToPosController(const LiveActor*, const char*, const sead::Vector3f*,
                                 const sead::Vector3f*, f32*, const sead::Vector3f&);
void initJointSpringController(const LiveActor*, const char*);
void initJointConstrainedSpringController(const LiveActor*, const char*);
void initJointSpringTransController(const LiveActor*, const char*);
void initJointSimpleIK(const LiveActor*, const char*);
}  // namespace al
