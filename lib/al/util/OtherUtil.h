#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

#include "al/util/InputUtil.h"

namespace sead {
class Heap;
}

namespace nn::ui2d {
class TextureInfo;
}

class PlayerActorHakoniwa;

namespace al {

class LiveActor;
class PlayerHolder;
class IUseCamera;
class Projection;
class IUseLayout;
class ActorInitInfo;
class Scene;
class AreaObj;
class IUseAudioKeeper;
class SensorMsg;
class IUseSceneObjHolder;
class HitSensor;
class PlacementInfo;

// from Starlight's header files. TODO clean this up, and include them in the proper places

sead::Heap* getCurrentHeap();

al::Projection* getProjection(al::IUseCamera const*, int);

int getSubActorNum(al::LiveActor const*);

al::LiveActor* getSubActor(al::LiveActor const*, int);

int getPlayerControllerPort(int);

char const* getActionName(al::LiveActor const*);

char const* getActionFrame(al::LiveActor const*);

sead::Vector3f* getCameraPos(al::IUseCamera const*, int);

sead::Vector3f* getSensorPos(al::LiveActor const*, const char*);

float calcSpeed(al::LiveActor const*);
float calcSpeedH(al::LiveActor const*);
float calcSpeedV(al::LiveActor const*);

float getSensorRadius(al::LiveActor const*, char const*);

// setters

void setTransY(al::LiveActor*, float);

void setTrans(al::LiveActor*, sead::Vector3f const&);

void setScaleAll(al::LiveActor*, float);

void setGravity(al::LiveActor const*, sead::Vector3f const&);

void setFront(al::LiveActor*, sead::Vector3f const&);

void setQuat(al::LiveActor*, const sead::Quatf&);

void setPaneTexture(al::IUseLayout*, char const*, nn::ui2d::TextureInfo const*);

void setSensorFollowPosOffset(al::LiveActor*, sead::Vector3f const&);

// void setPaneString(al::IUseLayout *layout, char const *paneName, char16_t const *, ushort);

void setPaneStringFormat(al::IUseLayout* layout, char const* paneName, char const* format, ...);

void setVelocityZero(al::LiveActor*);

// calc functions

f32 calcDistance(al::LiveActor const*,
                 al::LiveActor const*);  // calculates distance between two actors

f32 calcDistance(
    al::LiveActor const*,
    sead::Vector3f const&);  // calculates distance between an actor and a position in the world

// bools

bool isInAreaObj(al::LiveActor const*, const char*);

bool isInDeathArea(al::LiveActor const*);

bool getArg(int*, const al::ActorInitInfo&,
            const char*);  // gets an int argument from the actorinitinfo by a char* key

bool isActiveDemo(const al::Scene*);

bool isAreaTarget(al::LiveActor const*);

bool isSensorName(al::HitSensor const*, char const*);

// math

float powerIn(float base, float exponent);
float powerOut(float base, float exponent);

float squareIn(float value);

// misc

al::AreaObj* tryFindAreaObj(al::LiveActor const*, const char*);

bool tryGetAreaObjArg(int*, al::AreaObj const*, const char*);
bool tryGetAreaObjArg(float*, al::AreaObj const*, const char*);
bool tryGetAreaObjArg(bool*, al::AreaObj const*, const char*);

bool tryGetAreaObjStringArg(const char**, al::AreaObj const*, const char*);

bool tryGetArg(int*, const al::ActorInitInfo&, const char*);
bool tryGetArg(float*, const al::ActorInitInfo&, const char*);
bool tryGetArg(bool*, const al::ActorInitInfo&, const char*);
bool tryGetArg(int*, const al::PlacementInfo&, const char*);
bool tryGetArg(float*, const al::PlacementInfo&, const char*);
bool tryGetArg(bool*, const al::PlacementInfo&, const char*);

bool tryGetStringArg(const char**, al::ActorInitInfo const*, const char*);

bool isEqualString(const char* stringA, const char* stringB);

void offCollide(al::LiveActor*);
void onCollide(al::LiveActor*);

void startAction(al::LiveActor*, char const*);

bool tryStartSe(al::IUseAudioKeeper const*, sead::SafeStringBase<char> const&);

void startSe(al::IUseAudioKeeper const*, sead::SafeStringBase<char> const&);

void startHitReaction(al::LiveActor const*, char const*);

void calcCameraUpDir(sead::Vector3f*, al::IUseCamera const*, int);

const unsigned char* tryGetBymlFromArcName(sead::SafeStringBase<char> const&,
                                           sead::SafeStringBase<char> const&);

void initActor(al::LiveActor*, al::ActorInitInfo const&);

bool isObjectName(al::ActorInitInfo const&, char const*);
bool isObjectName(al::PlacementInfo const&, char const*);

void invalidateHitSensors(al::LiveActor*);

void hideModelIfShow(al::LiveActor*);
void showModelIfHide(al::LiveActor*);

void hideModel(al::LiveActor*);
void showModel(al::LiveActor*);

}  // namespace al

namespace rs {
uint32_t getStageShineAnimFrame(const al::LiveActor*, const char*);

PlayerActorHakoniwa* getPlayerActor(const al::Scene*);

bool isInChangeStageArea(al::LiveActor const*, sead::Vector3f const*);

bool isInvalidChangeStage(al::LiveActor const*);

bool isMsgCapTouchWall(al::SensorMsg const*);

void buyCap(al::IUseSceneObjHolder const*, char const*);

int getActiveQuestNum(al::IUseSceneObjHolder const*);
int getActiveQuestNo(al::IUseSceneObjHolder const*);
const char* getActiveQuestLabel(al::IUseSceneObjHolder const*);
void requestShowHtmlViewer(al::IUseSceneObjHolder const*);
}  // namespace rs
