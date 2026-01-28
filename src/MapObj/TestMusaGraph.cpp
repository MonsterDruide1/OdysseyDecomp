#include "MapObj/TestMusaGraph.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Rail/VertexGraph.h"

namespace {
NERVE_IMPL(TestMusaGraph, Wait)

NERVES_MAKE_STRUCT(TestMusaGraph, Wait)
}  // namespace

TestMusaGraph::TestMusaGraph(const char* name) : al::LiveActor(name) {}

// NON-MATCHING: Different stack pointer reference https://decomp.me/scratch/IDKIq
void TestMusaGraph::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initActorPoseTRSV(this);
    al::initActorSRT(this, info);
    al::initActorClipping(this, info);
    al::initExecutorUpdate(this, info, "地形オブジェ[Movement]");
    al::initStageSwitch(this, info);
    al::initNerve(this, &NrvTestMusaGraph.Wait, 0);
    al::initGroupClipping(this, info);
    makeActorAlive();

    al::Graph* graph = new al::Graph(0x40, 0x40);
    mGraph = graph;

    s32 railNum = al::calcLinkChildNum(info, "RailGraph");
    for (s32 i = 0; i < railNum; i++) {
        al::PlacementInfo placementInfo;
        al::getLinksInfoByIndex(&placementInfo, info, "RailGraph", i);
        al::Rail rail;
        rail.init(placementInfo);
        al::appendGraphFromRail<al::Graph::PosVertex, al::Graph::PosEdge>(graph, &rail, false,
                                                                          true);
    }

    al::Graph::PosVertex* vertex =
        new al::Graph::PosVertex(mGraph->getVertexCount(), sead::Vector3f::zero, 3);
    mGraph->appendVertex(vertex);

    mGraph->appendEdge(mGraph->getVertexCount() - 2, mGraph->getVertexCount() - 1, 1500.0f);
    mGraph->appendEdge(2, 8, 1000.0f);

    al::Graph::PosEdge* edge = reinterpret_cast<al::Graph::PosEdge*>(mGraph->tryFindEdge(0, 1));
    if (edge) {
        al::Graph::PosVertex* newEdge =
            new al::Graph::PosVertex(mGraph->getVertexCount(), {-1000.0f, 0.0f, -1000.0f}, 4);
        al::insertVertexAndSplitEdge<al::Graph::PosVertex, al::Graph::PosEdge>(mGraph, newEdge,
                                                                               edge);
    }

    sead::FixedObjArray<al::Graph::VertexInfo, 0x40> path{};
    al::calcShortestPath(&path, mGraph, 0, 8);
}

void TestMusaGraph::attackSensor(al::HitSensor* self, al::HitSensor* other) {}

bool TestMusaGraph::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                               al::HitSensor* self) {
    return false;
}

void TestMusaGraph::exeWait() {}
