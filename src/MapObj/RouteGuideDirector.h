#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"
#include "Library/Scene/ISceneObj.h"

namespace al {
struct ActorInitInfo;
class IUseSceneObjHolder;
class LiveActorGroup;
}  // namespace al
class RouteGuideArrowBase;
class RouteGuideHeadArrow;
class RouteGuidePoint;

class RouteGuideAreaFinder {
public:
    RouteGuideAreaFinder();
    void reset();
    virtual void findArea();

private:
    void* _8;
    sead::Vector3f _10;
    sead::Vector3f _1c;
    s32 _28;
};

class RouteGuideDirector : public al::LiveActor, public al::ISceneObj {
public:
    RouteGuideDirector();
    void initAfterPlacementSceneObj(const al::ActorInitInfo&) override;
    bool isValidate() const;
    void offGuideSystem();
    void deactivateGuide();
    void onGuideSystem();
    void activateGuide();
    void offGuideByActor(al::LiveActor*);
    void addInvidateList(al::LiveActor*);
    void onGuideByActor(al::LiveActor*);
    void removeInvidateList(al::LiveActor*);
    void addRouteGuidePointBufferCount(s32);
    void registerRouteGuidePoint(RouteGuidePoint*);
    void addRouteGuideArrowBufferCount(s32);
    void registerRouteGuideArrow(RouteGuideArrowBase*);
    void exeOff();
    void exeOn();

private:
    RouteGuidePoint** mPointBuffer;
    s32 mPointBufferCapacity;
    s32 mPointBufferCount;
    RouteGuideArrowBase** mArrowBuffer;
    s32 mArrowBufferCapacity;
    s32 mArrowBufferCount;
    RouteGuideAreaFinder* mAreaFinder;
    RouteGuideHeadArrow* mHeadArrow;
    void* _140;
    s32 _148;
    sead::Vector3f _14c;
    al::LiveActorGroup* mActorGroup;
    bool _160;
    bool _161;
};

namespace rs {
void offRouteGuideByActor(al::LiveActor*);
void onRouteGuideByActor(al::LiveActor*);
void offRouteGuideSystem(const al::IUseSceneObjHolder*);
void onRouteGuideSystem(const al::IUseSceneObjHolder*);
void createRouteGuideDirector(const al::IUseSceneObjHolder*);
void addRouteGuidePointBufferCount(const al::IUseSceneObjHolder*);
void registerRouteGuidePoint(const al::IUseSceneObjHolder*, RouteGuidePoint*);
void addRouteGuideArrowBufferCount(const al::IUseSceneObjHolder*, RouteGuideArrowBase*);
void registerRouteGuideArrow(const al::IUseSceneObjHolder*, RouteGuideArrowBase*);
}  // namespace rs

static_assert(sizeof(RouteGuideAreaFinder) == 0x30);
static_assert(sizeof(RouteGuideDirector) == 0x168);
