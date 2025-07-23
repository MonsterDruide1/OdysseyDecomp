#include "Npc/SmallBirdStateFlyAway.h"

#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/CollisionPartsTriangle.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/NpcAnimUtil.h"
#include "Util/SpecialBuildUtil.h"

namespace {
NERVE_IMPL(SmallBirdStateFlyAway, FlyAway);
NERVE_IMPL(SmallBirdStateFlyAway, FlyAwayHomeLanding);
NERVE_IMPL(SmallBirdStateFlyAway, FlyAwayKoopa);

NERVES_MAKE_STRUCT(SmallBirdStateFlyAway, FlyAway, FlyAwayHomeLanding, FlyAwayKoopa);
}  // namespace

SmallBirdStateFlyAway::SmallBirdStateFlyAway(al::LiveActor* actor)
    : BirdStateFlyAwayBase("小鳥の飛び去り", actor) {
    initNerve(&NrvSmallBirdStateFlyAway.FlyAway, 0);
}

void SmallBirdStateFlyAway::appear() {
    BirdStateFlyAwayBase::appear();
    mDisappearStep = al::getRandom(10) * 5 + 240;
    if (isFlyAwayByHomeLanding()) {
        mAngle = al::getRandom(45.0f, 70.0f);
        al::setNerve(this, &NrvSmallBirdStateFlyAway.FlyAwayHomeLanding);
    } else if (isFlyAwayByKoopa()) {
        mAngle = al::getRandom(45.0f, 70.0f);
        al::setNerve(this, &NrvSmallBirdStateFlyAway.FlyAwayKoopa);
    } else {
        mTargetDir.set(al::getFront(mActor));
        al::setNerve(this, &NrvSmallBirdStateFlyAway.FlyAway);
    }
}

static void startActionAtRandomFrameIfNotPlaying(al::LiveActor* actor, const char* action_name) {
    if (!al::isActionPlaying(actor, action_name))
        al::startActionAtRandomFrame(actor, action_name);
}

void SmallBirdStateFlyAway::startFlyAwayByKoopa() {
    BirdStateFlyAwayBase::startFlyAwayByKoopa();
    startActionAtRandomFrameIfNotPlaying(mActor, "Fly");
}

// TODO: Probably within `sead`
static void setLength(sead::Vector3f* vec, f32 length) {
    f32 old_length = vec->length();
    if (old_length > 0)
        *vec *= length / old_length;
}

static bool tryFaceToHorizontalDirection(al::LiveActor* actor, const sead::Vector3f& dir) {
    sead::Vector3f horizontal_dir = dir;
    al::verticalizeVec(&horizontal_dir, al::getGravity(actor), horizontal_dir);
    if (!al::tryNormalizeOrZero(&horizontal_dir))
        return false;
    al::faceToDirection(actor, horizontal_dir);
    return true;
}

// NON_MATCHING: wrong offset in store, should probably be ignored by tools/check
// (https://decomp.me/scratch/icRWQ)
void SmallBirdStateFlyAway::exeFlyAway() {
    if (al::isFirstStep(this)) {
        static const f32 gHorizontalAccel = 2.0f;
        static const f32 gE3MovieHorizontalAccel[] = {2.0f, 3.0f, 4.0f, 5.0f};
        static const f32 gVerticalAccel[] = {0.7f, 0.65f, 0.6f, 0.55f};
        static const f32 gE3MovieVerticalAccel[] = {3.0f, 3.5f, 4.0f, 4.5f, 5.0f, 5.5f};
        static s32 gVerticalAccelIndex = 0;
        static s32 gHorizontalAccelIndex = 0;
        static s32 gCollisionCheckOffsetStep = 0;

        mIsColliding = false;
        startActionAtRandomFrameIfNotPlaying(mActor, "Fly");
        if (rs::isModeE3MovieRom()) {
            gVerticalAccelIndex = al::modi(gVerticalAccelIndex + 7, 6);
            mVerticalAccel = gE3MovieVerticalAccel[gVerticalAccelIndex];
            gHorizontalAccelIndex = al::modi(gHorizontalAccelIndex + 5, 4);
            mHorizontalAccel = gE3MovieHorizontalAccel[gHorizontalAccelIndex];
        } else {
            gVerticalAccelIndex = al::modi(gVerticalAccelIndex + 5, 4);
            mVerticalAccel = gVerticalAccel[gVerticalAccelIndex];
            mHorizontalAccel = gHorizontalAccel;
        }
        mTargetAccelDir = {0, mVerticalAccel, mHorizontalAccel};
        al::normalize(&mTargetAccelDir);
        gCollisionCheckOffsetStep = al::modi(gCollisionCheckOffsetStep + 11, 10);
        mCollisionCheckOffsetStep = gCollisionCheckOffsetStep;
    }

    if (mHasTargetPos) {
        sead::Vector3f velocity = al::getVelocity(mActor);
        al::parallelizeVec(&velocity, al::getFront(mActor), velocity);
        *al::getVelocityPtr(mActor) -= velocity;

        sead::Vector3f target_dir = mTargetPos - al::getTrans(mActor);
        al::verticalizeVec(&target_dir, al::getGravity(mActor), target_dir);
        if (!al::tryNormalizeOrZero(&target_dir))
            target_dir.set(al::getFront(mActor));
        al::turnVecToVecDegree(&target_dir, al::getFront(mActor), target_dir, 15);
        al::turnVecToVecRate(&mTargetDir, target_dir, 0.2f);
        al::normalize(&mTargetDir);
        al::turnVecToVecRate(al::getFrontPtr(mActor), mTargetDir, 0.1f);
        al::normalize(al::getFrontPtr(mActor));

        *al::getVelocityPtr(mActor) += velocity.length() * al::getFront(mActor);
    }

    sead::Quatf quat = sead::Quatf::unit;
    al::calcQuat(&quat, mActor);

    sead::Vector3f accel = {0, mVerticalAccel, mHorizontalAccel};
    f32 accel_mag = accel.length();

    if (al::isFirstStep(this) || al::isIntervalStep(this, 10, mCollisionCheckOffsetStep)) {
        sead::Vector3f rotated = accel;
        rotated.rotate(quat);
        sead::Vector3f vec = rotated;
        f32 max_speed = al::calcSpeedMax(accel_mag, 0.97f);
        setLength(&vec, max_speed * 60);
        sead::Vector3f hit_pos = {0, 0, 0};
        al::Triangle triangle;
        mIsColliding = alCollisionUtil::getFirstPolyOnArrow(
            mActor, &hit_pos, &triangle, al::getTrans(mActor), vec, nullptr, nullptr);
        if (mIsColliding) {
            f32 distance = (hit_pos - al::getTrans(mActor)).length();
            if (distance < max_speed * 10 && !al::isFirstStep(this)) {
                bool turn_successful = tryFaceToHorizontalDirection(mActor, triangle.getNormal(0));
                if (!turn_successful)
                    mHorizontalAccel = 0;
            }
        }
    }

    sead::Vector3f accel_dir = {0, mVerticalAccel, mHorizontalAccel};
    al::normalize(&accel_dir);
    if (mIsColliding) {
        al::turnVecToVecDegree(&mTargetAccelDir, sead::Vector3f::ey, 2.5f);
        al::normalize(&mTargetAccelDir);
    }
    al::turnVecToVecRate(&accel_dir, mTargetAccelDir, 0.15f);
    al::normalize(&accel_dir);

    accel = accel_dir * accel_mag;
    mVerticalAccel = accel.y;
    mHorizontalAccel = accel.z;

    f32 old_accel = al::calcNerveValue(this, 1, 31, 2.0f, 0.8f);
    f32 new_accel = al::calcNerveValue(this, 30, 2.0f, 0.8f);
    mHorizontalAccel *= al::normalize(new_accel, 0.0f, old_accel);

    sead::Vector3f rotated = accel_dir;
    rotated.rotate(quat);
    al::setVelocity(mActor, rotated * al::calcSpeed(mActor));
    al::addVelocity(mActor, -al::getGravity(mActor) * mVerticalAccel +
                                al::getFront(mActor) * mHorizontalAccel);
    al::scaleVelocity(mActor, 0.97f);
    BirdFunction::tryUpdateFlyAwayDisappearDitherAlpha(mActor, this, mDisappearStep - 60,
                                                       mDisappearStep);
    if (al::isGreaterEqualStep(this, mDisappearStep))
        kill();
}

void SmallBirdStateFlyAway::exeFlyAwayHomeLanding() {
    if (al::isFirstStep(this))
        startActionAtRandomFrameIfNotPlaying(mActor, "Fly");
    al::addVelocity(mActor,
                    (al::getFront(mActor) * sead::Mathf::sin(sead::Mathf::deg2rad(mAngle)) -
                     al::getGravity(mActor) * sead::Mathf::cos(sead::Mathf::deg2rad(mAngle))) *
                        al::calcNerveValue(this, 30, 1.75f, 0.75f));
    al::scaleVelocity(mActor, 0.97f);
    BirdFunction::tryUpdateFlyAwayDisappearDitherAlpha(mActor, this, mDisappearStep - 60,
                                                       mDisappearStep);
    if (al::isGreaterEqualStep(this, mDisappearStep))
        kill();
}

void SmallBirdStateFlyAway::exeFlyAwayKoopa() {
    if (al::isFirstStep(this))
        startActionAtRandomFrameIfNotPlaying(mActor, "Fly");
    al::addVelocity(mActor,
                    (al::getFront(mActor) * sead::Mathf::sin(sead::Mathf::deg2rad(mAngle)) -
                     al::getGravity(mActor) * sead::Mathf::cos(sead::Mathf::deg2rad(mAngle))) *
                        al::calcNerveValue(this, 30, 5.5f, 4.0f));
    al::scaleVelocity(mActor, 0.97f);
    BirdFunction::tryUpdateFlyAwayDisappearDitherAlpha(mActor, this, mDisappearStep - 60,
                                                       mDisappearStep);
    if (al::isGreaterEqualStep(this, mDisappearStep))
        kill();
}
