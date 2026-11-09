#include "Camera/ScenarioStartCameraPoser.h"

#include <math/seadMathCalcCommon.h>

#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Rail/RailUtil.h"
#include "Library/Yaml/ByamlUtil.h"

ScenarioStartCameraPoser::ScenarioStartCameraPoser(const char* name, bool isUseDefault)
    : al::CameraPoser(name) {
    mInterpoleRate = 0.0f;
    mNextInterpoleStep = 0;
    mPlayStep = -1;
    mInterpoleType = 2;
    mEndWaitStep = 0;
    mIsUseDefault = isUseDefault;
}

void ScenarioStartCameraPoser::initScenarioCamera(const al::PlacementInfo& info) {
    al::tryGetArg(&mPlayStep, info, "PlayStep");
}

void ScenarioStartCameraPoser::loadParam(const al::ByamlIter& iter) {
    al::tryGetByamlS32(&mNextInterpoleStep, iter, "NextInterpoleStep");
    if (mIsUseDefault)
        return;
    al::tryGetByamlS32(&mInterpoleType, iter, "InterpoleType");
    al::tryGetByamlS32(&mEndWaitStep, iter, "EndWaitStep");
}

s32 ScenarioStartCameraPoser::getInterpoleType() const {
    if (mIsUseDefault)
        return 2;
    return mInterpoleType;
}

s32 ScenarioStartCameraPoser::getEndWaitStep() const {
    if (mIsUseDefault)
        return 90;
    return mEndWaitStep;
}

ScenarioStartCameraPoserSimpleZoom::ScenarioStartCameraPoserSimpleZoom(const char* name,
                                                                       bool isUseDefault)
    : ScenarioStartCameraPoser(name, isUseDefault) {
    mTargetTrans.set(0.0f, 0.0f, 0.0f);
    mStartDistance = 1800.0f;
    mEndDistance = 1800.0f;
    mAngleH = 0.0f;
    mAngleV = 30.0f;
}

void ScenarioStartCameraPoserSimpleZoom::initScenarioCamera(const al::PlacementInfo& info) {
    al::tryGetArg(&mPlayStep, info, "PlayStep");
    al::getTrans(&mTargetTrans, info);
}

void ScenarioStartCameraPoserSimpleZoom::loadParam(const al::ByamlIter& iter) {
    al::tryGetByamlS32(&mNextInterpoleStep, iter, "NextInterpoleStep");
    if (!mIsUseDefault) {
        al::tryGetByamlS32(&mInterpoleType, iter, "InterpoleType");
        al::tryGetByamlS32(&mEndWaitStep, iter, "EndWaitStep");
    }
    al::tryGetByamlF32(&mStartDistance, iter, "StartDistance");
    al::tryGetByamlF32(&mEndDistance, iter, "EndDistance");
    al::tryGetByamlF32(&mAngleH, iter, "AngleH");
    al::tryGetByamlF32(&mAngleV, iter, "AngleV");
}

void ScenarioStartCameraPoserSimpleZoom::update() {
    setAt(mTargetTrans);
    sead::Vector3f dir(sead::Mathf::sin(sead::Mathf::deg2rad(mAngleH)), 0.0f,
                        sead::Mathf::cos(sead::Mathf::deg2rad(mAngleH)));
    f32 cosV = sead::Mathf::cos(sead::Mathf::deg2rad(mAngleV));
    f32 len = dir.length();
    if (len > 0.0f)
        dir *= cosV / len;
    dir.y = sead::Mathf::sin(sead::Mathf::deg2rad(mAngleV));
    f32 dist = al::lerpValue(mStartDistance, mEndDistance, mInterpoleRate);
    f32 len2 = dir.length();
    if (len2 > 0.0f)
        dir *= dist / len2;
    setEye(getAt() + dir);
}

ScenarioStartCameraPoserRailMove::ScenarioStartCameraPoserRailMove(const char* name,
                                                                   bool isUseDefault)
    : ScenarioStartCameraPoser(name, isUseDefault) {
    mPointNum = 0;
    mPoints = nullptr;
}

void ScenarioStartCameraPoserRailMove::initScenarioCamera(const al::PlacementInfo& info) {
    al::tryGetArg(&mPlayStep, info, "PlayStep");
    al::PlacementInfo railInfo;
    al::getLinksInfo(&railInfo, info, "LookAtRail");
    initRail(railInfo);
    if (mPoints)
        return;
    mPointNum = al::getRailPointNum(this);
    mPoints = new Point*[mPointNum];
    for (s32 i = 0; i < mPointNum; i++) {
        al::PlacementInfo* pointInfo = al::getRailPointInfo(this, i);
        Point* point = new Point;
        point->mObjId = nullptr;
        point->mDistance = 1800.0f;
        point->mAngleH = 0.0f;
        point->mAngleV = 30.0f;
        mPoints[i] = point;
        al::PlacementId placementId;
        placementId.init(*pointInfo);
        mPoints[i]->mObjId = placementId.getId();
    }
}

void ScenarioStartCameraPoserRailMove::loadParam(const al::ByamlIter& iter) {
    al::tryGetByamlS32(&mNextInterpoleStep, iter, "NextInterpoleStep");
    if (!mIsUseDefault) {
        al::tryGetByamlS32(&mInterpoleType, iter, "InterpoleType");
        al::tryGetByamlS32(&mEndWaitStep, iter, "EndWaitStep");
    }
    al::ByamlIter pointListIter;
    if (iter.tryGetIterByKey(&pointListIter, "PointList")) {
        mPointNum = pointListIter.getSize();
        mPoints = new Point*[mPointNum];
        for (s32 i = 0; i < pointListIter.getSize(); i++) {
            al::ByamlIter pointIter;
            pointListIter.tryGetIterByIndex(&pointIter, i);
            Point* point = new Point;
            point->mObjId = nullptr;
            point->mDistance = 1800.0f;
            point->mAngleH = 0.0f;
            point->mAngleV = 30.0f;
            mPoints[i] = point;
            mPoints[i]->mObjId = al::getByamlKeyString(pointIter, "ObjId");
            mPoints[i]->mDistance = al::getByamlKeyFloat(pointIter, "Distance");
            mPoints[i]->mAngleH = al::getByamlKeyFloat(pointIter, "AngleH");
            mPoints[i]->mAngleV = al::getByamlKeyFloat(pointIter, "AngleV");
        }
    }
}

void ScenarioStartCameraPoserRailMove::update() {
    al::setRailPosToCoord(this, al::getRailTotalLength(this) * mInterpoleRate);
    setAt(al::getRailPos(this));
    s32 currentPointNo;
    if (al::getRailPointNum(this) >= 2) {
        currentPointNo = 1;
        do {
            if (al::getRailCoord(this) < al::calcRailCoordByPoint(this, currentPointNo))
                break;
            currentPointNo++;
        } while (currentPointNo < al::getRailPointNum(this));
        currentPointNo = al::getRailPointNum(this);
    } else {
        currentPointNo = al::getRailPointNum(this);
    }
    currentPointNo += (al::getRailPointNum(this) > currentPointNo) ? -1 : -2;
    s32 pointNum = al::getRailPointNum(this);
    s32 nextPointNo = al::modi(pointNum + currentPointNo + 1, pointNum);
    f32 t = al::normalize(al::getRailCoord(this), al::calcRailCoordByPoint(this, currentPointNo),
                          al::calcRailCoordByPoint(this, nextPointNo));
    sead::Vector3f dir(0.0f, 0.0f, 0.0f);
    sead::Vector3f dirA(sead::Mathf::sin(sead::Mathf::deg2rad(mPoints[currentPointNo]->mAngleH)),
                         0.0f,
                         sead::Mathf::cos(sead::Mathf::deg2rad(mPoints[currentPointNo]->mAngleH)));
    sead::Vector3f dirB(sead::Mathf::sin(sead::Mathf::deg2rad(mPoints[nextPointNo]->mAngleH)),
                         0.0f,
                         sead::Mathf::cos(sead::Mathf::deg2rad(mPoints[nextPointNo]->mAngleH)));
    al::turnVecToVecRate(&dir, dirA, dirB, t);
    f32 angleV = al::lerpValue(mPoints[currentPointNo]->mAngleV, mPoints[nextPointNo]->mAngleV, t);
    f32 cosV = sead::Mathf::cos(sead::Mathf::deg2rad(angleV));
    f32 len = dir.length();
    if (len > 0.0f)
        dir *= cosV / len;
    dir.y = sead::Mathf::sin(sead::Mathf::deg2rad(angleV));
    setEye(al::getRailPos(this) +
           dir * al::lerpValue(mPoints[currentPointNo]->mDistance,
                               mPoints[nextPointNo]->mDistance, t));
}
