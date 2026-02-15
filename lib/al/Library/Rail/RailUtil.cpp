#include "Library/Rail/RailUtil.h"

#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Rail/IUseRail.h"
#include "Library/Rail/Rail.h"
#include "Library/Rail/RailRider.h"

namespace al {

static RailRider* getRailRider(const IUseRail* railHolder) {
    return railHolder->getRailRider();
}

static const Rail* getRail(const IUseRail* railHolder) {
    return getRailRider(railHolder)->getRail();
}

void setRailPosToStart(IUseRail* railHolder) {
    getRailRider(railHolder)->moveToRailStart();
}

void setRailPosToEnd(IUseRail* railHolder) {
    getRailRider(railHolder)->moveToRailEnd();
}

void setRailPosToNearestPos(IUseRail* railHolder, const sead::Vector3f& pos) {
    getRailRider(railHolder)->moveToNearestRail(pos);
}

void setRailPosToCoord(IUseRail* railHolder, f32 coord) {
    getRailRider(railHolder)->setCoord(coord);
}

void setRailPosToRailPoint(IUseRail* railHolder, s32 index) {
    setRailPosToCoord(railHolder, calcRailCoordByPoint(railHolder, index));
}

void setSyncRailToStart(LiveActor* actor) {
    setRailPosToStart(actor);
    syncRailTrans(actor);
}

void syncRailTrans(LiveActor* actor) {
    setTrans(actor, getRailPos(actor));
}

void setSyncRailToEnd(LiveActor* actor) {
    setRailPosToEnd(actor);
    syncRailTrans(actor);
}

void setSyncRailToNearestPos(LiveActor* actor, const sead::Vector3f& pos) {
    setRailPosToNearestPos(actor, pos);
    syncRailTrans(actor);
}

void setSyncRailToNearestRailControlPoint(LiveActor* actor) {
    RailRider* railRider = getRailRider(actor);
    const Rail* rail = railRider->getRail();
    sead::Vector3f railPos;
    u32 index;
    rail->calcNearestRailPointPosFast(&railPos, &index, getTrans(actor));
    setTrans(actor, railPos);

    if (index == 0) {
        railRider->moveToRailStart();
        return;
    }
    railRider->moveToRailPoint(index);
}

void setSyncRailToNearestPos(LiveActor* actor) {
    setSyncRailToNearestPos(actor, getTrans(actor));
}

void setSyncRailToCoord(LiveActor* actor, f32 coord) {
    setRailPosToCoord(actor, coord);
    syncRailTrans(actor);
}

void setSyncRailToRailPoint(LiveActor* actor, s32 index) {
    setRailPosToRailPoint(actor, index);
    syncRailTrans(actor);
}

bool moveRail(IUseRail* railHolder, f32 speed) {
    RailRider* railRider = getRailRider(railHolder);
    railRider->setSpeed(speed);
    railRider->move();
    return isRailReachedGoal(railHolder);
}

bool isRailReachedGoal(const IUseRail* railHolder) {
    return getRailRider(railHolder)->isReachedGoal();
}

bool moveRailLoop(IUseRail* railHolder, f32 speed) {
    f32 coord = getRailCoord(railHolder);
    moveRail(railHolder, speed);
    if (isRailReachedGoal(railHolder)) {
        if (isRailGoingToEnd(railHolder))
            setRailPosToCoord(railHolder, (coord + speed) - getRailTotalLength(railHolder));
        else
            setRailPosToCoord(railHolder, getRailTotalLength(railHolder) - (speed - coord));
        return true;
    }
    return false;
}

f32 getRailCoord(const IUseRail* railHolder) {
    return getRailRider(railHolder)->getCoord();
}

bool isRailGoingToEnd(const IUseRail* railHolder) {
    return getRailRider(railHolder)->isMoveForwards();
}

f32 getRailTotalLength(const IUseRail* railHolder) {
    return getRail(railHolder)->getTotalLength();
}

bool moveRailTurn(IUseRail* railHolder, f32 speed, f32 goalCoord) {
    if (speed < 0.0f)
        reverseRail(railHolder);

    moveRail(railHolder, sead::Mathf::abs(speed));
    bool isReversed = goalCoord <= 0.0f ? isRailReachedGoal(railHolder) :
                                          isRailReachedNearGoal(railHolder, goalCoord);

    if (isReversed)
        reverseRail(railHolder);
    if (speed < 0.0f)
        reverseRail(railHolder);
    return isReversed;
}

void reverseRail(IUseRail* railHolder) {
    getRailRider(railHolder)->reverse();
}

bool isRailReachedNearGoal(const IUseRail* railHolder, f32 goalCoord) {
    return isRailReachedNearGoal(railHolder, goalCoord, goalCoord);
}

bool turnToRailDir(LiveActor* actor, f32 deg) {
    sead::Vector3f moveDir = {0.0f, 0.0f, 0.0f};
    calcRailMoveDir(&moveDir, actor);
    sead::Quatf* quat = tryGetQuatPtr(actor);

    if (quat)
        return turnQuatFrontToDirDegreeH(actor, moveDir, deg);
    return turnDirectionDegree(actor, getFrontPtr(actor), moveDir, deg);
}

void calcRailMoveDir(sead::Vector3f* moveDir, const IUseRail* railHolder) {
    moveDir->set(isRailGoingToEnd(railHolder) ? getRailDir(railHolder) : -getRailDir(railHolder));
}

bool turnToRailDirImmediately(LiveActor* actor) {
    return turnToRailDir(actor, 180.0f);
}

const sead::Vector3f& getRailPos(const IUseRail* railHolder) {
    return getRailRider(railHolder)->getPosition();
}

bool moveSyncRail(LiveActor* actor, f32 speed) {
    bool isReachedGoal = moveRail(actor, speed);
    syncRailTrans(actor);
    return isReachedGoal;
}

bool moveSyncRailLoop(LiveActor* actor, f32 speed) {
    bool isReachedGoal = moveRailLoop(actor, speed);
    syncRailTrans(actor);
    if (isReachedGoal)
        resetPosition(actor);
    return isReachedGoal;
}

bool moveSyncRailTurn(LiveActor* actor, f32 speed) {
    bool isReversed = moveRailTurn(actor, speed, 0.0f);
    syncRailTrans(actor);
    return isReversed;
}

f32 calcNearestRailCoord(const IUseRail* railHolder, const sead::Vector3f& pos) {
    return getRail(railHolder)->calcNearestRailPosCoord(pos, 20.0f);
}

f32 calcNearestRailPos(sead::Vector3f* railPos, const IUseRail* railHolder,
                       const sead::Vector3f& pos) {
    return getRail(railHolder)->calcNearestRailPos(railPos, pos, 20.0f);
}

void calcNearestRailDir(sead::Vector3f* dir, const IUseRail* railHolder,
                        const sead::Vector3f& pos) {
    f32 coord = calcNearestRailCoord(railHolder, pos);
    calcRailDirAtCoord(dir, railHolder, coord);
}

void calcNearestRailPosAndDir(sead::Vector3f* railPos, sead::Vector3f* dir,
                              const IUseRail* railHolder, const sead::Vector3f& pos) {
    f32 coord = calcNearestRailCoord(railHolder, pos);
    calcRailPosDir(railPos, dir, railHolder, coord);
}

void calcRailPosDir(sead::Vector3f* railPos, sead::Vector3f* dir, const IUseRail* railHolder,
                    f32 coord) {
    getRail(railHolder)->calcPosDir(railPos, dir, coord);
}

void calcRailPointPos(sead::Vector3f* pos, const IUseRail* railHolder, s32 index) {
    getRail(railHolder)->calcRailPointPos(pos, index);
}

s32 calcNearestRailPointNo(const IUseRail* railHolder, const sead::Vector3f& pos) {
    s32 index = 0;
    getRail(railHolder)->calcNearestRailPointNo(&index, pos);
    return index;
}

s32 calcNearestRailPointNo(const IUseRail* railHolder) {
    return calcNearestRailPointNo(railHolder, getRailPos(railHolder));
}

s32 calcCoordNearestRailPointNo(const IUseRail* railHolder) {
    s32 index = getRailPartIndex(railHolder);
    f32 rate = calcRailPartRate(railHolder);

    if (isRailGoingToEnd(railHolder))
        index += rate > 0.5f;
    else
        index += rate < 0.5f;

    s32 pointNo = index;
    if (isLoopRail(railHolder) && index == getRailPointNum(railHolder))
        pointNo = 0;
    return pointNo;
}

s32 getRailPartIndex(const IUseRail* railHolder) {
    return getRail(railHolder)->getIncludedSectionIndex(getRailCoord(railHolder));
}

f32 calcRailPartRate(const IUseRail* railHolder) {
    const Rail* rail = getRail(railHolder);
    f32 partDistance = 0.0f;
    f32 length = 0.0f;
    f32 sectionLength =
        rail->getIncludedSectionLength(&partDistance, &length, getRailCoord(railHolder));
    if (isRailGoingToEnd(railHolder))
        return partDistance / sectionLength;
    return length / sectionLength;
}

bool isLoopRail(const IUseRail* railHolder) {
    return getRail(railHolder)->isClosed();
}

s32 getRailPointNum(const IUseRail* railHolder) {
    return getRail(railHolder)->getRailPointsCount();
}

f32 calcNearestRailPointPosCoord(const IUseRail* railHolder, const sead::Vector3f& pos) {
    return calcRailCoordByPoint(railHolder, calcNearestRailPointNo(railHolder, pos));
}

f32 calcRailCoordByPoint(const IUseRail* railHolder, s32 index) {
    return getRail(railHolder)->getLengthToPoint(index);
}

void calcRailUp(sead::Vector3f* up, const IUseRail* railHolder) {
    sead::Vector3f currentAngle = {0.0f, 0.0f, 0.0f};
    sead::Vector3f nextAngle = {0.0f, 0.0f, 0.0f};

    tryGetRotate(&currentAngle, *getRailPointInfo(railHolder, getRailPointNo(railHolder)));
    tryGetRotate(&nextAngle, *getRailPointInfo(railHolder, getNextRailPointNo(railHolder)));

    sead::Matrix34f rotationMatrix;
    rotationMatrix.makeR({sead::Mathf::deg2rad(currentAngle.x),
                          sead::Mathf::deg2rad(currentAngle.y),
                          sead::Mathf::deg2rad(currentAngle.z)});
    sead::Vector3f currentUp = rotationMatrix.getBase(1);

    rotationMatrix.makeR({sead::Mathf::deg2rad(nextAngle.x), sead::Mathf::deg2rad(nextAngle.y),
                          sead::Mathf::deg2rad(nextAngle.z)});
    sead::Vector3f nextUp = rotationMatrix.getBase(1);

    f32 rate = calcRailPartRate(railHolder);
    up->set(currentUp * (1.0f - rate) + nextUp * rate);
    normalize(up);
}

PlacementInfo* getRailPointInfo(const IUseRail* railHolder, s32 index) {
    return getRail(railHolder)->getRailPoint(index);
}

s32 getRailPointNo(const IUseRail* railHolder) {
    if (isLoopRail(railHolder))
        return getRailPartIndex(railHolder);

    if (isRailReachedEnd(railHolder))
        return getRailPointNum(railHolder) - 1;

    return getRailPartIndex(railHolder);
}

s32 getNextRailPointNo(const IUseRail* railHolder) {
    s32 modifier = isRailGoingToEnd(railHolder) ? 1 : -1;

    bool isLoop = isLoopRail(railHolder);
    s32 newIndex = getRailPointNo(railHolder) + modifier;
    s32 railPointNum = getRailPointNum(railHolder);

    if (isLoop) {
        s32 sum = railPointNum + newIndex;
        s32 railPointNumAgain = getRailPointNum(railHolder);
        return modi(sum + railPointNumAgain, railPointNumAgain);
    }

    return sead::Mathi::clamp2(0, newIndex, railPointNum - 1);
}

f32 calcRailToGoalLength(const IUseRail* railHolder) {
    const Rail* rail = getRail(railHolder);
    f32 length = rail->getTotalLength();
    if (rail->isClosed())
        return length;

    f32 coord = getRailCoord(railHolder);
    if (isRailGoingToEnd(railHolder))
        return length - coord;

    return coord;
}

f32 calcRailTotalRate(const IUseRail* railHolder) {
    f32 coord = getRailCoord(railHolder);
    f32 length = getRailTotalLength(railHolder);
    return normalize(coord, 0.0f, length);
}

f32 calcRailToNextRailPointLength(const IUseRail* railHolder) {
    const Rail* rail = getRail(railHolder);
    f32 length = 0.0f;
    f32 partDistance = 0.0f;
    rail->getIncludedSectionLength(&partDistance, &length, getRailCoord(railHolder));

    if (isRailGoingToEnd(railHolder))
        return length;
    return partDistance;
}

f32 calcRailToPreviousRailPointLength(const IUseRail* railHolder) {
    const Rail* rail = getRail(railHolder);
    f32 length = 0.0f;
    f32 partDistance = 0.0f;
    rail->getIncludedSectionLength(&partDistance, &length, getRailCoord(railHolder));

    if (isRailGoingToEnd(railHolder))
        return partDistance;
    return length;
}

s32 getRailNum(const IUseRail* railHolder) {
    return getRail(railHolder)->getRailPartCount();
}

const sead::Vector3f& getRailDir(const IUseRail* railHolder) {
    return getRailRider(railHolder)->getDirection();
}

bool isRailReachedEnd(const IUseRail* railHolder) {
    return getRailRider(railHolder)->isReachedRailEnd();
}

f32 getRailPartLength(const IUseRail* railHolder, s32 index) {
    return getRail(railHolder)->getPartLength(index);
}

bool tryGetCurrentRailPointArg(f32* arg, const IUseRail* railHolder, const char* argName) {
    s32 pointNo = getRailPointNo(railHolder);
    return tryGetArg(arg, *getRailPointInfo(railHolder, pointNo), argName);
}

bool tryGetNextRailPointArg(f32* arg, const IUseRail* railHolder, const char* argName) {
    s32 pointNo = getNextRailPointNo(railHolder);
    return tryGetArg(arg, *getRailPointInfo(railHolder, pointNo), argName);
}

bool tryGetCurrentRailPointArg(s32* arg, const IUseRail* railHolder, const char* argName) {
    s32 pointNo = getRailPointNo(railHolder);
    return tryGetArg(arg, *getRailPointInfo(railHolder, pointNo), argName);
}

bool tryGetNextRailPointArg(s32* arg, const IUseRail* railHolder, const char* argName) {
    s32 pointNo = getNextRailPointNo(railHolder);
    return tryGetArg(arg, *getRailPointInfo(railHolder, pointNo), argName);
}

bool tryGetCurrentRailPointArg(bool* arg, const IUseRail* railHolder, const char* argName) {
    s32 pointNo = getRailPointNo(railHolder);
    return tryGetArg(arg, *getRailPointInfo(railHolder, pointNo), argName);
}

bool tryGetNextRailPointArg(bool* arg, const IUseRail* railHolder, const char* argName) {
    s32 pointNo = getNextRailPointNo(railHolder);
    return tryGetArg(arg, *getRailPointInfo(railHolder, pointNo), argName);
}

bool tryGetCurrentRailPointArg(const char** arg, const IUseRail* railHolder, const char* argName) {
    s32 pointNo = getRailPointNo(railHolder);
    return tryGetStringArg(arg, *getRailPointInfo(railHolder, pointNo), argName);
}

bool tryGetNextRailPointArg(const char** arg, const IUseRail* railHolder, const char* argName) {
    s32 pointNo = getNextRailPointNo(railHolder);
    return tryGetStringArg(arg, *getRailPointInfo(railHolder, pointNo), argName);
}

bool isExistRail(const IUseRail* railHolder) {
    return getRailRider(railHolder) != nullptr;
}

bool isRailReachedStart(const IUseRail* railHolder) {
    return getRailRider(railHolder)->isReachedRailStart();
}

bool isRailReachedNearGoal(const IUseRail* railHolder, f32 goalCoordStart, f32 goalCoordEnd) {
    if (isLoopRail(railHolder))
        return false;

    if (isRailGoingToEnd(railHolder)) {
        if (getRailTotalLength(railHolder) - goalCoordEnd <= getRailCoord(railHolder))
            return true;
    } else {
        if (goalCoordStart >= getRailCoord(railHolder))
            return true;
    }
    return false;
}

bool isRailReachedEdge(const IUseRail* railHolder) {
    return getRailRider(railHolder)->isReachedEdge();
}

bool isRailReachedNearRailPoint(const IUseRail* railHolder, f32 epsilon) {
    return getRail(railHolder)->isNearRailPoint(getRailCoord(railHolder), epsilon);
}

bool isRailIncludeBezierPart(const IUseRail* railHolder) {
    return getRail(railHolder)->isIncludeBezierRailPart();
}

bool isRailBezierPart(const IUseRail* railHolder, s32 index) {
    return getRail(railHolder)->isBezierRailPart(index);
}

bool isRailPlusDir(const IUseRail* railHolder, const sead::Vector3f& dir) {
    return dir.dot(getRailDir(railHolder)) >= 0.0f;
}

bool isRailPlusPoseSide(const LiveActor* actor) {
    sead::Vector3f sideDir;
    calcSideDir(&sideDir, actor);
    return isRailPlusDir(actor, sideDir);
}

bool isRailPlusPoseUp(const LiveActor* actor) {
    sead::Vector3f upDir;
    calcUpDir(&upDir, actor);
    return isRailPlusDir(actor, upDir);
}

bool isRailPlusPoseFront(const LiveActor* actor) {
    sead::Vector3f frontDir;
    calcFrontDir(&frontDir, actor);
    return isRailPlusDir(actor, frontDir);
}

void calcRailPosAtCoord(sead::Vector3f* pos, const IUseRail* railHolder, f32 coord) {
    getRail(railHolder)->calcPos(pos, coord);
}

void calcRailDirAtCoord(sead::Vector3f* dir, const IUseRail* railHolder, f32 coord) {
    getRail(railHolder)->calcDirection(dir, coord);
}

inline f32 modLimit(f32 value, f32 limit) {
    return modf(value + limit, limit) + 0.0f;
}

// Mismatch: https://decomp.me/scratch/0vmMR
void calcRailPosFront(sead::Vector3f* pos, const IUseRail* railHolder, f32 offset) {
    if (!isRailGoingToEnd(railHolder))
        offset = -offset;

    f32 coordOffset = offset + getRailCoord(railHolder);
    const Rail* rail = getRail(railHolder);

    if (rail->isClosed()) {
        f32 totalLength = rail->getTotalLength();
        f32 distance = modLimit(coordOffset, totalLength);
        rail->calcPos(pos, distance);
        return;
    }

    f32 length = rail->getTotalLength();
    f32 beep = (coordOffset <= length) ? coordOffset : 0.0f;
    rail->calcPos(pos, (coordOffset < 0.0f) ? beep : length);
}

// Mismatch: https://decomp.me/scratch/0vmMR
void calcRailDirFront(sead::Vector3f* pos, const IUseRail* railHolder, f32 offset) {
    if (!isRailGoingToEnd(railHolder))
        offset = -offset;

    f32 coordOffset = offset + getRailCoord(railHolder);
    const Rail* rail = getRail(railHolder);

    if (rail->isClosed()) {
        f32 totalLength = rail->getTotalLength();
        f32 distance = modLimit(coordOffset, totalLength);
        rail->calcDirection(pos, distance);
        return;
    }

    f32 length = rail->getTotalLength();
    f32 beep = (coordOffset <= length) ? coordOffset : 0.0f;
    rail->calcDirection(pos, (coordOffset < 0.0f) ? beep : length);
}

void calcRailPointPose(sead::Quatf* pose, const IUseRail* railHolder, s32 index) {
    getQuat(pose, *getRailPointInfo(railHolder, index));
}

s32 calcRailPointNum(const IUseRail* railHolder, f32 distance1, f32 distance2) {
    return getRail(railHolder)->calcRailPointNum(distance1, distance2);
}

s32 calcForwardRailPointNo(const IUseRail* railHolder, s32 index) {
    s32 pointNum = getRailPointNum(railHolder);
    if (isRailGoingToEnd(railHolder)) {
        if (pointNum - 1 == index)
            return isLoopRail(railHolder) ? 0 : -1;

        return index + 1;
    } else {
        if (index == 0)
            return isLoopRail(railHolder) ? pointNum - 1 : -1;

        return index - 1;
    }
}

s32 calcPreviousRailPointNo(const IUseRail* railHolder, s32 index) {
    s32 pointNum = getRailPointNum(railHolder);
    if (isRailGoingToEnd(railHolder)) {
        if (index == 0)
            return isLoopRail(railHolder) ? pointNum - 1 : -1;

        return index - 1;
    } else {
        if (pointNum - 1 == index)
            return isLoopRail(railHolder) ? 0 : -1;

        return index + 1;
    }
}

void calcRailClippingInfo(sead::Vector3f* pos, f32* distance, const IUseRail* railHolder, f32 step,
                          f32 offset) {
    f32 totalLength = getRailTotalLength(railHolder);
    s32 stepNum = totalLength / step;

    sead::Vector3f railPos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f lastRailPos = {0.0f, 0.0f, 0.0f};
    calcRailPosAtCoord(&railPos, railHolder, 0.0f);
    calcRailPosAtCoord(&lastRailPos, railHolder, totalLength);

    sead::BoundBox3f boundBox{
        {sead::Mathf::min(railPos.x, lastRailPos.x), sead::Mathf::min(railPos.y, lastRailPos.y),
         sead::Mathf::min(railPos.z, lastRailPos.z)},
        {sead::Mathf::max(railPos.x, lastRailPos.x), sead::Mathf::max(railPos.y, lastRailPos.y),
         sead::Mathf::max(railPos.z, lastRailPos.z)},
    };

    for (s32 index = 1; index < stepNum; index++) {
        sead::Vector3f helper = {0.0f, 0.0f, 0.0f};
        getRail(railHolder)->calcPos(&helper, index * step);

        boundBox.addPoint(helper);
    }

    pos->set(boundBox.getCenter());
    *distance = 0.0f;

    for (s32 index = 0; index < stepNum; index++) {
        sead::Vector3f helper = {0.0f, 0.0f, 0.0f};
        getRail(railHolder)->calcPos(&helper, index * step);
        *distance = sead::Mathf::max((helper - *pos).length(), *distance);
    }
    *distance = sead::Mathf::max((lastRailPos - *pos).length(), *distance);

    *distance += offset;
}

void setRailClippingInfo(sead::Vector3f* pos, LiveActor* actor, f32 step, f32 offset) {
    f32 distance = 0.0f;
    calcRailClippingInfo(pos, &distance, actor, step, offset);
    setClippingInfo(actor, distance, pos);
}

void calcAndSyncMoveFitRailDir(IUseRail* railHolder, sead::Vector3f* outDir, f32 step,
                               const sead::Vector3f& pos, const sead::Vector3f& gravity,
                               bool reverse) {
    setRailPosToNearestPos(railHolder, pos);

    RailRider* railRider = getRailRider(railHolder);
    sead::Vector3f railDir = getRailDir(railHolder);
    if (reverse)
        railDir.negate();

    sead::Vector3f railDirH;
    verticalizeVec(&railDirH, gravity, railDir);
    tryNormalizeOrZero(&railDirH);

    sead::Vector3f posOff = railRider->getPosition() - pos;
    sead::Vector3f posDirH;
    verticalizeVec(&posDirH, gravity, posOff);

    f32 posOffDistance;
    separateScalarAndDirection(&posOffDistance, &posDirH, posDirH);

    lerpVec(outDir, railDirH, posDirH, sead::Mathf::clamp(posOffDistance / step, 0.0f, 1.0f));
    tryNormalizeOrZero(outDir);
}

void calcAndSyncMoveFitRailDir(LiveActor* actor, sead::Vector3f* outDir, f32 step, bool reverse) {
    calcAndSyncMoveFitRailDir(actor, outDir, step, getTrans(actor), getGravity(actor), reverse);
}

LiveActorGroup* createRailModelGroup(const IUseRail* railHolder, const ActorInitInfo& initInfo,
                                     const char* actorName, const char* archiveName) {
    s32 railPointNum = getRailPointNum(railHolder);
    getRailRider(railHolder);
    LiveActorGroup* group = new LiveActorGroup("レールモデルグループ", railPointNum);

    for (s32 i = 0; i < railPointNum; i++) {
        sead::Vector3f pointPos = {0.0f, 0.0f, 0.0f};
        calcRailPointPos(&pointPos, railHolder, i);

        if (i == 0) {
            if (!isLoopRail(railHolder))
                continue;
            sead::Vector3f prevPointPos = {0.0f, 0.0f, 0.0f};
            calcRailPointPos(&prevPointPos, railHolder, railPointNum - 1);
            LiveActor* actor =
                createRailModelActor(initInfo, pointPos, prevPointPos, actorName, archiveName);
            group->registerActor(actor);
        }

        if (i > 0) {
            sead::Vector3f prevPointPos = {0.0f, 0.0f, 0.0f};
            calcRailPointPos(&prevPointPos, railHolder, i - 1);
            LiveActor* actor =
                createRailModelActor(initInfo, prevPointPos, pointPos, actorName, archiveName);
            group->registerActor(actor);
        }
    }

    return group;
}

LiveActor* createRailModelActor(const ActorInitInfo& info, const sead::Vector3f& prevPointPos,
                                const sead::Vector3f& pointPos, const char* actorName,
                                const char* archiveName) {
    LiveActor* actor = new LiveActor(actorName);
    initActorWithArchiveName(actor, info, archiveName, nullptr);
    actor->makeActorAlive();

    sead::Vector3f front = pointPos;
    front -= prevPointPos;
    f32 frontLength = front.length();
    front.normalize();

    sead::Quatf quat;
    makeQuatFrontUp(&quat, front,
                    isParallelDirection(front, sead::Vector3f::ey) ? sead::Vector3f::ex :
                                                                     sead::Vector3f::ey);
    updatePoseQuat(actor, quat);

    resetPosition(actor, pointPos - frontLength * front * 0.5f);
    setScale(actor, 1.0f, 1.0f, frontLength * 0.01f);

    return actor;
}

s32 calcRailDivideNum(const IUseRail* railHolder, f32 segmentLength, bool isForce) {
    if (isForce)
        return calcRailDivideForcePointNum(railHolder, segmentLength);
    else
        return calcRailDivideEquallyNum(railHolder, segmentLength);
}

s32 calcRailDivideForcePointNum(const IUseRail* railHolder, f32 segmentLength) {
    s32 railNum = getRailNum(railHolder);
    s32 i;
    s32 total = 0;

    for (i = 0; i < railNum; i++)
        total += (s32)(getRailPartLength(railHolder, i) / segmentLength) + 1;

    // For looping rails, Rail::init already counts up by one
    if (!isLoopRail(railHolder))
        total += 1;

    return total;
}

s32 calcRailDivideEquallyNum(const IUseRail* railHolder, f32 segmentLength) {
    return (s32)(getRailTotalLength(railHolder) / segmentLength) + 1;
}

void placementRailDivide(const IUseRail* railHolder, f32 segmentLength, bool isForce,
                         const RailPlacementCallBack& callback) {
    if (isForce)
        placementRailDivideForcePoint(railHolder, segmentLength, callback);
    else
        placementRailDivideEqually(railHolder, segmentLength, callback);
}

void placementRailDivideForcePoint(const IUseRail* railHolder, f32 segmentLength,
                                   const RailPlacementCallBack& callback) {
    s32 railNum = getRailNum(railHolder);

    f32 coord = 0.0f;
    s32 index = 0;
    for (s32 i = 0; i < railNum; i++) {
        f32 length = getRailPartLength(railHolder, i);
        s32 railNum = (s32)(length / segmentLength) + 1;
        for (s32 e = 0; e < railNum; e++) {
            sead::Vector3f vec = sead::Vector3f::zero;
            calcRailPosAtCoord(&vec, railHolder, coord);
            callback(vec, index, coord);
            index++;
            coord += length / railNum;
        }
    }

    // Additional rail also listed in calcRailDivideForcePointNum
    if (!isLoopRail(railHolder)) {
        sead::Vector3f vec = sead::Vector3f::zero;
        f32 totalLength = getRailTotalLength(railHolder);
        calcRailPosAtCoord(&vec, railHolder, totalLength);
        callback(vec, index, totalLength);
    }
}

void placementRailDivideEqually(const IUseRail* railHolder, f32 segmentLength,
                                const RailPlacementCallBack& callback) {
    f32 length = getRailTotalLength(railHolder);
    s32 railNum = (s32)(length / segmentLength) + 1;

    f32 coord = 0.0f;
    for (s32 i = 0; i < railNum; i++) {
        sead::Vector3f vec = sead::Vector3f::zero;
        calcRailPosAtCoord(&vec, railHolder, coord);
        callback(vec, i, coord);
        coord += length / railNum;
    }
}

}  // namespace al
