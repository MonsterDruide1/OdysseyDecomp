#pragma once

#include <basis/seadTypes.h>
#include <container/seadObjArray.h>
#include <container/seadPtrArray.h>
#include <math/seadVector.h>

#include "Library/Rail/Rail.h"

namespace al {

class Graph {
public:
    class Edge;
    class Vertex;
    class PosEdge;
    class PosVertex;
    struct VertexInfo;

    Graph(s32 verticesSize, s32 edgesSize);
    void appendVertex(s32 size);
    void appendVertex(Vertex* vertex);
    void removeVertex(const Vertex* vertex);
    void removeEdge(const Edge* edge);
    Edge* tryFindEdge(s32 indexVertex1, s32 indexVertex2) const;
    void appendEdge(Edge* edge);
    bool tryAppendEdge(Edge* edge);
    void appendEdge(s32 indexVertex1, s32 indexVertex2, f32 weight);
    bool tryAppendEdge(s32 indexVertex1, s32 indexVertex2, f32 weight);

    s32 getVertexCount() const { return mVertices.size(); }

    Vertex* getVertex(s32 index) const { return mVertices[index]; }

    s32 getEdgeCount() const { return mEdges.size(); }

    Edge* getEdge(s32 index) const { return mEdges[index]; }

private:
    sead::PtrArray<Vertex> mVertices;
    sead::PtrArray<Edge> mEdges;
};

static_assert(sizeof(Graph) == 0x20);

class Graph::Vertex {
public:
    Vertex(s32 size, s32 index) : mIndex(index) { mEdges.allocBuffer(size, nullptr); }

    void addEdge(Edge* edge) { mEdges.pushBack(edge); }

    void tryAddEdge(Edge* edge) {
        for (s32 i = 0; i < mEdges.size(); i++)
            if (mEdges[i] == edge)
                return;
        addEdge(edge);
    }

    s32 getEdgeCount() const { return mEdges.size(); }

    Edge* getEdge(s32 index) const { return mEdges[index]; }

    void eraseEdge(s32 index) { mEdges.erase(index); }

    s32 getIndex() const { return mIndex; }

private:
    sead::PtrArray<Edge> mEdges;
    s32 mIndex;
};

static_assert(sizeof(Graph::Vertex) == 0x18);

class Graph::Edge {
public:
    Edge(Vertex* vertex1, Vertex* vertex2, f32 weight)
        : mVertex1(vertex1), mVertex2(vertex2), mWeight(weight) {}

    virtual f32 getWeight() const { return mWeight; }

    Vertex* getVertex1() const { return mVertex1; }

    void setVertex1(Vertex* vertex) { mVertex1 = vertex; }

    Vertex* getVertex2() const { return mVertex2; }

    void setVertex2(Vertex* vertex) { mVertex2 = vertex; }

private:
    Vertex* mVertex1;
    Vertex* mVertex2;
    f32 mWeight;
};

static_assert(sizeof(Graph::Edge) == 0x20);

class Graph::PosVertex : public Vertex {
public:
    PosVertex(s32 index, const sead::Vector3f& pos, s32 size = 10)
        : Vertex(size, index), mPos(pos) {}

    const sead::Vector3f& getPos() const { return mPos; }

    sead::Vector3f* getPosPtr() { return &mPos; }

private:
    sead::Vector3f mPos;
};

static_assert(sizeof(Graph::PosVertex) == 0x20);

class Graph::PosEdge : public Edge {
public:
    PosEdge(PosVertex* vertex1, PosVertex* vertex2) : Edge(vertex1, vertex2, 0.0f) {
        mDir = vertex2->getPos() - vertex1->getPos();
        mLength = mDir.length();

        mDir.normalize();
    }

    f32 getWeight() const override {
        sead::Vector3f v1Pos = getVertex1()->getPos();
        sead::Vector3f v2Pos = getVertex2()->getPos();
        return (v2Pos - v1Pos).length();
    }

    PosVertex* getVertex1() const { return static_cast<PosVertex*>(Edge::getVertex1()); }

    PosVertex* getVertex2() const { return static_cast<PosVertex*>(Edge::getVertex2()); }

private:
    sead::Vector3f mDir;
    f32 mLength;
};

static_assert(sizeof(Graph::PosEdge) == 0x30);

struct Graph::VertexInfo {
    Vertex* vertex;
    s32 prevIndex;
    f32 weight;

    inline bool operator<(const VertexInfo& rhs) const {
        return vertex->getIndex() < rhs.vertex->getIndex();
    }
};

static_assert(sizeof(Graph::VertexInfo) == 0x10);

bool calcShortestPath(sead::ObjArray<Graph::VertexInfo>* vertexInfos, const Graph* graph,
                      s32 startVertexIndex, s32 endVertexIndex);
f32 calcDistanceAndNearestPos(sead::Vector3f* outPos, const Graph::PosEdge* edge,
                              const sead::Vector3f& pos);
Graph::Edge* findEdgeMinimumWeight(const Graph* graph);
Graph::Edge* findEdgeMinimumWeight(const Graph::Vertex* vertex);
s32 findEdgeMinimumWeight(const Graph::Edge** outEdge,
                          const sead::ConstPtrArray<Graph::Edge>& edges);
Graph::Edge* tryFindEdgeStartVertex(const Graph::Vertex* vertexA, const Graph::Vertex* vertexB);
Graph::Edge* tryFindEdgeEndVertex(const Graph::Vertex* vertexA, const Graph::Vertex* vertexB);
Graph::PosVertex* findNearestPosVertex(const Graph* graph, const sead::Vector3f& pos,
                                       f32 maxDistance);
Graph::PosVertex* findFarthestPosVertex(const Graph* graph, const sead::Vector3f& pos,
                                        f32 minDistance);
Graph::PosEdge* findPosEdgeByVertexPosUndirect(const Graph* graph, const sead::Vector3f& posA,
                                               const sead::Vector3f& posB);
Graph::PosEdge* findPosEdgeByVertexPos(const Graph* graph, const sead::Vector3f& posA,
                                       const sead::Vector3f& posB);
bool isDestinationVertex(const Graph::PosVertex* vertexA, const Graph::PosVertex* vertexB);
bool tryRemoveEdgeFromVertex(Graph::Vertex* vertex, Graph::Edge* edge);
bool isExistVertex(const Graph* graph, const Graph::Vertex* vertex);
bool isSpanningTree(const Graph* graph);
bool isSpanningTree(const sead::ConstPtrArray<Graph::Edge>& edges);
bool isWalk(const sead::ConstPtrArray<Graph::Edge>& edges);
bool isTrail(const sead::ConstPtrArray<Graph::Edge>& edges);
bool isCircuit(const sead::ConstPtrArray<Graph::Edge>& edges);
bool isPath(const sead::ConstPtrArray<Graph::Edge>& edges);
bool isCycle(const sead::ConstPtrArray<Graph::Edge>& edges);
void calcMinimumSpanningTree(sead::ConstPtrArray<Graph::Edge>* edges, const Graph* graph);

template <typename Vertex, typename Edge>
void createAndAppendEdge(Graph* graph, Vertex* vertexA, Vertex* vertexB, bool isBidirectional) {
    if (!graph->tryFindEdge(vertexA->getIndex(), vertexB->getIndex()))
        graph->appendEdge(new Edge(vertexA, vertexB));

    if (isBidirectional && !graph->tryFindEdge(vertexB->getIndex(), vertexA->getIndex()))
        graph->appendEdge(new Edge(vertexB, vertexA));
}

template <typename Vertex, typename Edge>
void appendGraphFromRail(Graph* graph, const Rail* rail, bool disableEdgeForBezier,
                         bool isBidirectional) {
    s32 railPoints = rail->getRailPointsCount();

    Vertex* firstVertex = nullptr;
    Vertex* lastVertex = nullptr;
    for (s32 i = 0; i < railPoints; i++) {
        sead::Vector3f pointPos;
        rail->calcRailPointPos(&pointPos, i);
        Vertex* currentVertex = nullptr;

        for (s32 j = 0; j < graph->getVertexCount(); j++) {
            Vertex* v = reinterpret_cast<Vertex*>(graph->getVertex(j));
            sead::Vector3f vPos = v->getPos();
            if ((vPos - pointPos).squaredLength() < 100.0f)
                currentVertex = v;
        }

        if (!currentVertex) {
            currentVertex = new Vertex(graph->getVertexCount(), pointPos);
            graph->appendVertex(currentVertex);
        }

        if (!firstVertex)
            firstVertex = currentVertex;

        if (lastVertex) {
            if (lastVertex != currentVertex &&
                !(disableEdgeForBezier && rail->isBezierRailPart(i - 1)))
                createAndAppendEdge<Vertex, Edge>(graph, lastVertex, currentVertex,
                                                  isBidirectional);

            if (i == railPoints - 1 && rail->isClosed() &&
                !(disableEdgeForBezier && rail->isBezierRailPart(i)))
                createAndAppendEdge<Vertex, Edge>(graph, currentVertex, firstVertex,
                                                  isBidirectional);
        }

        lastVertex = currentVertex;
    }
}

template <typename Vertex, typename Edge>
Edge* insertVertexAndSplitEdge(Graph* graph, Vertex* newVertex, Edge* edge) {
    Vertex* previousVertex = edge->getVertex2();
    edge->setVertex2(newVertex);

    tryRemoveEdgeFromVertex(previousVertex, edge);
    if (!isExistVertex(graph, newVertex))
        graph->appendVertex(newVertex);

    Edge* newEdge =
        reinterpret_cast<Edge*>(graph->tryFindEdge(newVertex->getIndex(), previousVertex->getIndex())) ?:
            new Edge(newVertex, previousVertex);

    if (!tryFindEdgeEndVertex(newVertex, previousVertex))
        previousVertex->addEdge(newEdge);

    graph->tryAppendEdge(newEdge);

    if (!tryFindEdgeStartVertex(newVertex, previousVertex))
        newVertex->addEdge(newEdge);

    if (!tryFindEdgeEndVertex(edge->getVertex1(), newVertex))
        newVertex->addEdge(edge);

    return newEdge;
}
}  // namespace al
