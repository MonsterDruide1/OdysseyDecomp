#include "Player/PlayerActionSlopeSlideControl.h"

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathUtil.h"

#include "Player/PlayerConst.h"
#include "Player/PlayerInput.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerCollisionUtil.h"

PlayerActionSlopeSlideControl::PlayerActionSlopeSlideControl(al::LiveActor* player,
                                                             const PlayerConst* pConst,
                                                             const PlayerInput* input,
                                                             const IUsePlayerCollision* collider)
    : mPlayer(player), mConst(pConst), mInput(input), mCollision(collider) {}

void PlayerActionSlopeSlideControl::setup() {
    mGroundNormal = -al::getGravity(mPlayer);
    mDirSlide = {0.0f, 0.0f, 0.0f};

    if (rs::isCollidedGround(mCollision)) {
        al::calcDirSlide(&mDirSlide, al::getGravity(mPlayer),
                         rs::getCollidedGroundNormal(mCollision));
        if (!rs::isJustLand(mCollision))
            mGroundNormal = rs::getCollidedGroundNormal(mCollision);
    }

    mHorizontalVelocity = {0.0f, 0.0f, 0.0f};
}

void PlayerActionSlopeSlideControl::setupCutSlideOppositeDir() {
    setup();

    sead::Vector3f groundNormal = {0.0f, 0.0f, 0.0f};
    rs::calcGroundNormalOrGravityDir(&groundNormal, mPlayer, mCollision);

    sead::Vector3f slideDir = {0.0f, 0.0f, 0.0f};
    sead::Vector3f* velocityPtr = al::getVelocityPtr(mPlayer);
    al::alongVectorNormalH(velocityPtr, *velocityPtr, mGroundNormal, groundNormal);
    if (rs::calcSlideDir(&slideDir, al::getGravity(mPlayer), groundNormal))
        al::limitVectorOppositeDir(velocityPtr, slideDir, *velocityPtr, velocityPtr->length());

    *velocityPtr -= mConst->getGravityMove() * groundNormal;
    mGroundNormal = groundNormal;
}

f32 PlayerActionSlopeSlideControl::update(f32 accel, f32 brake, f32 against, f32 anglePowerMax,
                                          f32 maxSpeed, f32 sideAccel, f32 sideBrake,
                                          f32 sideMaxSpeed, f32 turnDegree, f32 gravity,
                                          bool isRolling) {
    sead::Vector3f prevGroundNormal = mGroundNormal;
    sead::Vector3f up = -al::getGravity(mPlayer);
    sead::Vector3f velocity = al::getVelocity(mPlayer);
    bool isOnGroundSlopeSlideEnd = rs::isOnGroundSlopeSlideEnd(mPlayer, mCollision, mConst);
    if (rs::isOnGround(mPlayer, mCollision)) {
        mGroundNormal = rs::getCollidedGroundNormal(mCollision);
        up = mGroundNormal;
        al::calcDirSlide(&mDirSlide, al::getGravity(mPlayer), up);
        al::alongVectorNormalH(&velocity, velocity, prevGroundNormal, up);

        sead::Vector3f velDirSlide = {0.0f, 0.0f, 0.0f};
        sead::Vector3f velSide = {0.0f, 0.0f, 0.0f};
        if (isOnGroundSlopeSlideEnd) {
            al::separateVectorParallelVertical(&velSide, &velDirSlide, up, velocity);
            velDirSlide *= brake;
            velSide *= sideBrake;
            if (isRolling) {
                sead::Vector3f moveInput = {0.0f, 0.0f, 0.0f};
                mInput->calcMoveInput(&moveInput, up);

                sead::Vector3f normDirSlide = {0.0f, 0.0f, 0.0f};
                al::tryNormalizeOrZero(&normDirSlide, velDirSlide);

                sead::Vector3f side;
                side.setCross(up, normDirSlide);
                al::tryNormalizeOrZero(&side);

                f32 sideInput = side.dot(moveInput);
                sead::Vector3f moveSideVec = side * sideInput * sideAccel;
                al::addVectorLimit(&velDirSlide, moveSideVec, maxSpeed);
            }
        } else {
            al::separateVectorParallelVertical(&velDirSlide, &velSide, mDirSlide, velocity);
            sead::Vector3f moveInput = {0.0f, 0.0f, 0.0f};
            mInput->calcMoveInput(&moveInput, up);
            f32 speedDirSlide = velDirSlide.dot(mDirSlide);
            if (speedDirSlide < 0.0)
                velDirSlide *= brake;

            f32 moveInputAgainst = sead::Mathf::clamp(-mDirSlide.dot(moveInput), 0.0f, 1.0f);
            f32 minSpeedNorm = sead::Mathf::clamp(1.0f - (moveInputAgainst * against), 0.0, 1.0);
            f32 speedNorm = sead::Mathf::clamp(speedDirSlide + 1.0f, minSpeedNorm, 1.0f);

            f32 anglePower = sead::Mathf::clamp(
                90.0f - al::calcAngleDegree(mDirSlide, al::getGravity(mPlayer)), 0.0f, 90.0f);

            f32 anglePowerNorm = 1.0f;
            if (anglePowerMax > 0.0)
                anglePowerNorm = sead::Mathf::clampMax(anglePower / anglePowerMax, 1.0f);

            al::addVectorLimit(&velDirSlide, mDirSlide * accel * anglePowerNorm * speedNorm,
                               maxSpeed);
            velSide *= sideBrake;
            sead::Vector3f inputDir = {0.0f, 0.0f, 0.0f};
            if (al::tryNormalizeOrZero(&inputDir, moveInput)) {
                sead::Vector3f sideDir;
                sideDir.setCross(up, mDirSlide);
                al::addVectorLimit(&velSide,
                                   sideDir * sideDir.dot(inputDir) * moveInput.length() * sideAccel,
                                   sideMaxSpeed);
            }
        }

        sead::Vector3f vel;
        vel.x = velDirSlide.x + velSide.x;
        vel.y = velDirSlide.y + velSide.y;
        vel.z = velDirSlide.z + velSide.z;
        al::limitLength(&vel, vel, maxSpeed);
        al::setVelocity(mPlayer, vel - (mConst->getGravityMove() * up));
    } else {
        mGroundNormal = up;
        f32 prevUpVel = prevGroundNormal.dot(al::getVelocity(mPlayer));
        al::alongVectorNormalH(al::getVelocityPtr(mPlayer), al::getVelocity(mPlayer),
                               prevGroundNormal, mGroundNormal);
        sead::Vector3f* velPtr = al::getVelocityPtr(mPlayer);
        velPtr->x = (prevUpVel * mGroundNormal.x) + velPtr->x;
        velPtr->y = (prevUpVel * mGroundNormal.y) + velPtr->y;
        velPtr->z = (prevUpVel * mGroundNormal.z) + velPtr->z;
        al::addVectorLimit(al::getVelocityPtr(mPlayer), up * -gravity, mConst->getFallSpeedMax());
    }

    sead::Vector3f hVel = {0.0f, 0.0f, 0.0f};
    al::verticalizeVec(&hVel, up, al::getVelocity(mPlayer));
    sead::Vector3f hVelDir = {0.0f, 0.0f, 0.0f};

    f32 rumbleStrength;
    if (al::tryNormalizeOrZero(&hVelDir, hVel)) {
        sead::Vector3f front = {0.0f, 0.0f, 0.0f};
        al::calcFrontDir(&front, mPlayer);
        al::verticalizeVec(&front, up, front);
        al::tryNormalizeOrZero(&front);
        if (turnDegree > 0.0) {
            f32 angleFrontVel = 2 * al::calcAngleDegree(front, hVelDir);
            f32 turn = sead::Mathf::clamp(angleFrontVel / turnDegree, 0.0f, 1.0f);
            sead::Vector3f upFrontVel;
            upFrontVel.setCross(front, hVelDir);
            rumbleStrength = -al::sign(up.dot(upFrontVel)) * turn;
        } else {
            rumbleStrength = 0.0;
        }

        al::turnVecToVecCosOnPlane(&front, hVelDir, up,
                                   sead::Mathf::cos(sead::Mathf::deg2rad(turnDegree)));
        rs::slerpUpFront(mPlayer, up, front, mConst->getSlerpQuatRate(),
                         mConst->getHillPoseDegreeMax());
    } else {
        rumbleStrength = 0.0;
    }

    mHorizontalVelocity = hVel;
    return rumbleStrength;
}
