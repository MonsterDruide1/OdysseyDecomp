#pragma once

#include <basis/seadTypes.h>
#include <container/seadObjArray.h>
#include <container/seadPtrArray.h>
#include <math/seadVector.h>

namespace al {

class Graph {
public:
    class Edge;
    class PosVertex;
    class PosEdge;
    class Vertex;
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
    inline Vertex(s32 size, s32 index) : mIndex(index) { mEdges.allocBuffer(size, nullptr); }

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

    Vertex* getVertex2() const { return mVertex2; }

private:
    Vertex* mVertex1;
    Vertex* mVertex2;
    f32 mWeight;
};

static_assert(sizeof(Graph::Edge) == 0x20);

class Graph::PosVertex : public Vertex {
public:
    const sead::Vector3f& getPos() const { return mPos; }

private:
    sead::Vector3f mPos;
};

static_assert(sizeof(Graph::PosVertex) == 0x20);

class Graph::PosEdge : public Edge {
public:
    f32 getWeight() const override {
        const sead::Vector3f& pos1 = ((PosVertex*)getVertex1())->getPos();
        const sead::Vector3f& pos2 = ((PosVertex*)getVertex2())->getPos();
        return (pos2 - pos1).length();
    }

private:
    sead::Vector3f mPos;
    f32 mPosLength;
};

static_assert(sizeof(Graph::PosEdge) == 0x30);

struct Graph::VertexInfo {
    Vertex* vertex;
    s32 index;
    f32 weight;
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

}  // namespace al
