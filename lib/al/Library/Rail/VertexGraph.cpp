#include "Library/Rail/VertexGraph.h"

namespace al {

Graph::Graph(s32 vertices_size, s32 edges_size) {
    mVertices.allocBuffer(vertices_size, nullptr);
    mEdges.allocBuffer(edges_size, nullptr);
}

void Graph::appendVertex(s32 size) {
    appendVertex(new Vertex(size, mVertices.size()));
}

void Graph::appendVertex(Vertex* vertex) {
    mVertices.pushBack(vertex);
}

void Graph::removeVertex(const Vertex* vertex) {
    for (s32 i = 0; i < mVertices.size(); i++) {
        if (mVertices[i] == vertex) {
            for (s32 i = 0; i < vertex->getEdgeCount(); i++)
                removeEdge(vertex->getEdge(i));
            mVertices.erase(i);
            return;
        }
    }
}

void Graph::removeEdge(const Edge* edge) {
    for (s32 i = 0; i < mEdges.size(); i++) {
        if (mEdges[i] == edge) {
            mEdges.erase(i);
            return;
        }
    }
}

Graph::Edge* Graph::tryFindEdge(s32 index_vertex1, s32 index_vertex2) const {
    for (s32 i = 0; i < mEdges.size(); i++) {
        auto* edge = mEdges[i];
        if (edge->getVertex1()->getIndex() == index_vertex1 &&
            edge->getVertex2()->getIndex() == index_vertex2)
            return edge;
    }
    return nullptr;
}

void Graph::appendEdge(Edge* edge) {
    edge->getVertex1()->addEdge(edge);
    edge->getVertex2()->addEdge(edge);
    mEdges.pushBack(edge);
}

bool Graph::tryAppendEdge(Edge* edge) {
    if (tryFindEdge(edge->getVertex1()->getIndex(), edge->getVertex2()->getIndex()))
        return false;

    edge->getVertex1()->tryAddEdge(edge);
    edge->getVertex2()->tryAddEdge(edge);
    mEdges.pushBack(edge);
    return true;
}

void Graph::appendEdge(s32 index_vertex1, s32 index_vertex2, f32 weight) {
    Vertex* vertex1 = mVertices[index_vertex1];
    Vertex* vertex2 = mVertices[index_vertex2];
    Edge* edge = new Edge(vertex1, vertex2, weight);

    vertex1->addEdge(edge);
    vertex2->addEdge(edge);
    mEdges.pushBack(edge);
}

bool Graph::tryAppendEdge(s32 index_vertex1, s32 index_vertex2, f32 weight) {
    if (tryFindEdge(index_vertex1, index_vertex2))
        return false;

    Vertex* vertex1 = mVertices[index_vertex1];
    Vertex* vertex2 = mVertices[index_vertex2];
    Edge* edge = new Edge(vertex1, vertex2, weight);

    vertex1->addEdge(edge);
    vertex2->addEdge(edge);
    mEdges.pushBack(edge);
    return true;
}

void calcShortestPath(sead::ObjArray<Graph::VertexInfo>*, const Graph*, s32, s32);
f32 calcDistanceAndNearestPos(sead::Vector3f*, const Graph::PosEdge*, const sead::Vector3f&);

Graph::Edge* findEdgeMinimumWeight(const Graph* graph) {
    Graph::Edge* minEdge = nullptr;

    f32 minWeight = sead::Mathf::maxNumber();
    for (s32 i = 0; i < graph->getEdgeCount(); i++) {
        Graph::Edge* edge = graph->getEdge(i);
        if (edge->getWeight() < minWeight) {
            minWeight = edge->getWeight();
            minEdge = edge;
        }
    }

    return minEdge;
}

Graph::Edge* findEdgeMinimumWeight(const Graph::Vertex* vertex) {
    Graph::Edge* minEdge = nullptr;

    f32 minWeight = sead::Mathf::maxNumber();
    for (s32 i = 0; i < vertex->getEdgeCount(); i++) {
        Graph::Edge* edge = vertex->getEdge(i);
        if (edge->getWeight() < minWeight) {
            minWeight = edge->getWeight();
            minEdge = edge;
        }
    }

    return minEdge;
}

s32 findEdgeMinimumWeight(const Graph::Edge** outEdge,
                          const sead::ConstPtrArray<Graph::Edge>& edges) {
    Graph::Edge* minEdge = nullptr;

    f32 minWeight = sead::Mathf::maxNumber();
    f32 minEdgeIndex = -1.0f;
    for (s32 i = 0; i < edges.size(); i++) {
        Graph::Edge* edge = edges[i];
        if (edge->getWeight() < minWeight) {
            minWeight = edge->getWeight();
            minEdgeIndex = i;
            minEdge = edge;
        }
    }

    if (outEdge)
        *outEdge = minEdge;

    return minEdgeIndex;
}

Graph::Edge* tryFindEdgeStartVertex(const Graph::Vertex* vertexA, const Graph::Vertex* vertexB) {
    for (s32 i = 0; i < vertexA->getEdgeCount(); i++) {
        Graph::Edge* edge = vertexA->getEdge(i);
        if (edge->getVertex2() == vertexB)
            return edge;
    }

    return nullptr;
}

Graph::Edge* tryFindEdgeEndVertex(const Graph::Vertex* vertexA, const Graph::Vertex* vertexB) {
    for (s32 i = 0; i < vertexB->getEdgeCount(); i++) {
        Graph::Edge* edge = vertexB->getEdge(i);
        if (edge->getVertex1() == vertexA)
            return edge;
    }

    return nullptr;
}

Graph::Vertex* findNearestPosVertex(const Graph* graph, const sead::Vector3f& pos, f32 value) {
    // for (s32 i = 0; i < graph->getVertexCount(); i++)
    //    Graph::Vertex* edge = graph->getVertex(i);
}

Graph::Vertex* findFarthestPosVertex(const Graph*, const sead::Vector3f&, f32);

Graph::PosEdge* findPosEdgeByVertexPosUndirect(const Graph* graph, const sead::Vector3f& posA,
                                               const sead::Vector3f& posB) {
    for (s32 i = 0; i < graph->getEdgeCount(); i++) {
        Graph::PosEdge* edge = (Graph::PosEdge*)graph->getEdge(i);
        Graph::PosVertex* vertex1 = (Graph::PosVertex*)edge->getVertex1();
        Graph::PosVertex* vertex2 = (Graph::PosVertex*)edge->getVertex2();
        sead::Vector3f pos1 = vertex1->getPos() - posA;
        sead::Vector3f pos2 = vertex2->getPos() - posB;
        if (pos1.length() < 10.0f && pos2.length() < 10.0f)
            return edge;

        sead::Vector3f pos3 = vertex1->getPos() - posB;
        sead::Vector3f pos4 = vertex2->getPos() - posA;
        if (pos3.length() < 10.0f && pos4.length() < 10.0f)
            return edge;
    }

    return nullptr;
}

Graph::PosEdge* findPosEdgeByVertexPos(const Graph* graph, const sead::Vector3f& posA,
                                       const sead::Vector3f& posB) {
    for (s32 i = 0; i < graph->getEdgeCount(); i++) {
        Graph::PosEdge* edge = (Graph::PosEdge*)graph->getEdge(i);
        Graph::PosVertex* vertex1 = (Graph::PosVertex*)edge->getVertex1();
        Graph::PosVertex* vertex2 = (Graph::PosVertex*)edge->getVertex2();
        sead::Vector3f pos1 = vertex1->getPos() - posA;
        sead::Vector3f pos2 = vertex2->getPos() - posB;
        if (pos1.length() < 10.0f && pos2.length() < 10.0f)
            return edge;
    }

    return nullptr;
}

bool isDestinationVertex(const Graph::PosVertex* vertexA, const Graph::PosVertex* vertexB) {
    for (s32 i = 0; i < vertexA->getEdgeCount(); i++)
        if (vertexA->getEdge(i)->getVertex1() == vertexA &&
            vertexA->getEdge(i)->getVertex2() == vertexB)
            return true;
    return false;
}

bool tryRemoveEdgeFromVertex(Graph::Vertex* vertex, Graph::Edge* edge) {
    for (s32 i = 0; i < vertex->getEdgeCount(); i++) {
        if (vertex->getEdge(i) == edge) {
            vertex->eraseEdge(i);
            return true;
        }
    }
    return false;
}

bool isExistVertex(const Graph* graph, const Graph::Vertex* vertex) {
    s32 vertexCount = graph->getVertexCount();
    for (s32 i = 0; i < vertexCount; i++)
        if (graph->getVertex(i) == vertex)
            return true;
    return false;
}

bool isSpanningTree(const Graph* graph) {
    return true;
}

bool isSpanningTree(const sead::ConstPtrArray<Graph::Edge>& edges) {
    return true;
}

inline bool isClosed(const sead::ConstPtrArray<Graph::Edge>& edges) {
    return edges[0]->getVertex1() == edges[edges.size() - 1]->getVertex2();
}

bool isWalk(const sead::ConstPtrArray<Graph::Edge>& edges) {
    for (s32 i = 1; i < edges.size(); i++)
        if (edges[i - 1]->getVertex2() != edges[i]->getVertex1())
            return false;
    return true;
}

bool isTrail(const sead::ConstPtrArray<Graph::Edge>& edges) {
    if (!isWalk(edges))
        return false;

    s32 size = edges.size();
    for (s32 i = 0; i < size; i++) {
        for (s32 j = i + 1; j < size; j++) {
            if (edges[i]->getVertex2() == edges[j]->getVertex1())
                if (edges[i]->getVertex1() == edges[j]->getVertex2())
                    return false;

            if (edges[i]->getVertex1() == edges[j]->getVertex1())
                if (edges[i]->getVertex2() == edges[j]->getVertex2())
                    return false;
        }
    }
    return true;
}

bool isCircuit(const sead::ConstPtrArray<Graph::Edge>& edges) {
    return isTrail(edges) && isClosed(edges);
}

bool isPath(const sead::ConstPtrArray<Graph::Edge>& edges) {
    if (!isTrail(edges))
        return false;

    s32 size = edges.size();
    for (s32 i = 0; i < size; i++) {
        for (s32 j = i + 1; j < size; j++)
            if (edges[i]->getVertex2() == edges[j]->getVertex2())
                return false;
    }

    return true;
}

bool isCycle(const sead::ConstPtrArray<Graph::Edge>& edges) {
    return isPath(edges) && isClosed(edges);
}

void calcMinimumSpanningTree(sead::ConstPtrArray<Graph::Edge>* edges, const Graph* graph) {}

}  // namespace al
