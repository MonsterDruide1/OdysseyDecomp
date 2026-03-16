#include "Library/Rail/VertexGraph.h"

#include "Library/Math/MathUtil.h"

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
            for (s32 j = 0; j < vertex->getEdgeCount(); j++)
                removeEdge(vertex->getEdge(j));
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

// Note: This is Dijkstra's algorithm
// NON_MATCHING: Different implementation https://decomp.me/scratch/7pP7o
bool calcShortestPath(sead::ObjArray<Graph::VertexInfo>* vertexInfos, const Graph* graph,
                      s32 startVertexIndex, s32 endVertexIndex) {
    sead::FixedObjArray<Graph::VertexInfo, 512> unvisitedSet;

    // Assign to every node a distance from start value: for the starting node, it is zero,
    // and for all other nodes, it is infinity, since initially no path is known to these nodes
    for (s32 i = 0; i < graph->getVertexCount(); i++) {
        Graph::VertexInfo info = {
            .vertex = graph->getVertex(i),
            .prevIndex = -1,
            .weight = i == startVertexIndex ? 0.0f : sead::Mathf::maxNumber(),
        };
        unvisitedSet.emplaceBack(info);
    }

    Graph::Vertex* currentVertex = graph->getVertex(startVertexIndex);

    while (unvisitedSet.size() > 0) {
        bool found = false;
        s32 currentIndex = -1;
        Graph::VertexInfo* currentInfo = nullptr;

        for (s32 i = 0; i < unvisitedSet.size(); i++) {
            currentInfo = unvisitedSet[i];
            if (currentInfo->vertex == currentVertex) {
                currentIndex = i;
                found = true;
                break;
            }
        }

        if (found) {
            vertexInfos->emplaceBack(*currentInfo);

            // For the current node, consider all of its neighbors and update their distances
            for (s32 i = 0; i < currentInfo->vertex->getEdgeCount(); i++) {
                Graph::Edge* edge = currentInfo->vertex->getEdge(i);
                if (edge->getVertex1() != currentInfo->vertex)
                    continue;

                Graph::Vertex* neighbor = edge->getVertex2();
                for (s32 j = 0; j < unvisitedSet.size(); j++) {
                    Graph::VertexInfo* vInfo = unvisitedSet[j];
                    neighbor = edge->getVertex2();
                    if (neighbor != vInfo->vertex)
                        continue;
                    f32 weight = currentInfo->weight + edge->getWeight();
                    if (weight < vInfo->weight) {
                        vInfo->prevIndex = currentInfo->vertex->getIndex();
                        vInfo->weight = weight;
                    }
                }
            }

            // After considering all of the current node's unvisited neighbors,
            // the current node is removed from the unvisited set
            unvisitedSet.erase(currentIndex);
        }

        // Select the current node to be the one with the smallest distance
        f32 minWeight = sead::Mathf::maxNumber();
        for (s32 i = 0; i < unvisitedSet.size(); i++) {
            if (unvisitedSet[i]->weight < minWeight) {
                minWeight = unvisitedSet[i]->weight;
                currentVertex = unvisitedSet[i]->vertex;
            }
        }

        if (!found)
            return false;

        // If we are on our goal index we are done
        if (currentVertex->getIndex() == endVertexIndex) {
            while (unvisitedSet.size() > 0) {
                vertexInfos->emplaceBack(*unvisitedSet[0]);
                unvisitedSet.erase(0);
            }
            vertexInfos->sort();
            return true;
        }
    }

    return false;
}

f32 calcDistanceAndNearestPos(sead::Vector3f* outPos, const Graph::PosEdge* edge,
                              const sead::Vector3f& pos) {
    sead::Vector3f edgeDir0 = edge->getVertex2()->getPos();
    edgeDir0 -= edge->getVertex1()->getPos();

    f32 originalEdgeLength = edgeDir0.length();
    sead::Vector3f edgeDir = edgeDir0;

    if (!tryNormalizeOrZero(&edgeDir))
        return -1.0f;

    sead::Vector3f posDir = pos;
    posDir -= edge->getVertex1()->getPos();
    f32 dot = edgeDir.dot(posDir);

    sead::Vector3f nearestPos;
    if (dot < 0.0f) {
        nearestPos.set(edge->getVertex1()->getPos());
    } else if (dot > originalEdgeLength) {
        nearestPos.set(edge->getVertex2()->getPos());
    } else {
        nearestPos.set(edge->getVertex1()->getPos());
        nearestPos += dot * edgeDir;
    }

    sead::Vector3f posOff;
    verticalizeVec(&posOff, edgeDir, posDir);

    if (outPos)
        outPos->set(nearestPos);

    return (nearestPos - pos).length();
}

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
    // BUG: should have been an `s32`
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

// NON_MATCHING: Inverted registers https://decomp.me/scratch/CxUxe
Graph::PosVertex* findNearestPosVertex(const Graph* graph, const sead::Vector3f& pos,
                                       f32 maxDistance) {
    Graph::PosVertex* nearestVertex = nullptr;
    f32 minDistance = sead::Mathf::maxNumber();
    for (s32 i = 0; i < graph->getVertexCount(); i++) {
        Graph::PosVertex* vertex = reinterpret_cast<Graph::PosVertex*>(graph->getVertex(i));
        f32 distance = (vertex->getPos() - pos).length();
        if (maxDistance < 0.0f && distance < minDistance) {
            nearestVertex = vertex;
            minDistance = distance;
        }

        if (maxDistance >= 0.0f && distance < minDistance && distance < maxDistance) {
            nearestVertex = vertex;
            minDistance = distance;
        }
    }
    return nearestVertex;
}

// NON_MATCHING: Same as findNearestPosVertex but inverted
Graph::PosVertex* findFarthestPosVertex(const Graph* graph, const sead::Vector3f& pos,
                                        f32 minDistance) {
    Graph::PosVertex* farthestVertex = nullptr;
    f32 maxDistance = 0.0f;
    for (s32 i = 0; i < graph->getVertexCount(); i++) {
        Graph::PosVertex* vertex = reinterpret_cast<Graph::PosVertex*>(graph->getVertex(i));
        f32 distance = (vertex->getPos() - pos).length();
        if (minDistance < 0.0f && distance > maxDistance) {
            farthestVertex = vertex;
            maxDistance = distance;
        }

        if (minDistance >= 0.0f && distance > maxDistance && distance > minDistance) {
            farthestVertex = vertex;
            maxDistance = distance;
        }
    }
    return farthestVertex;
}

Graph::PosEdge* findPosEdgeByVertexPosUndirect(const Graph* graph, const sead::Vector3f& posA,
                                               const sead::Vector3f& posB) {
    for (s32 i = 0; i < graph->getEdgeCount(); i++) {
        Graph::PosEdge* edge = reinterpret_cast<Graph::PosEdge*>(graph->getEdge(i));
        Graph::PosVertex* vertex1 = edge->getVertex1();
        Graph::PosVertex* vertex2 = edge->getVertex2();
        sead::Vector3f posV1 = vertex1->getPos();
        sead::Vector3f posV2 = vertex2->getPos();
        sead::Vector3f posDiff1 = posV1 - posA;
        sead::Vector3f posDiff2 = posV2 - posB;
        if (posDiff1.length() < 10.0f && posDiff2.length() < 10.0f)
            return edge;

        sead::Vector3f posV3 = vertex1->getPos();
        sead::Vector3f posV4 = vertex2->getPos();
        sead::Vector3f posDiff3 = posV3 - posB;
        sead::Vector3f posDiff4 = posV4 - posA;
        if (posDiff3.length() < 10.0f && posDiff4.length() < 10.0f)
            return edge;
    }

    return nullptr;
}

Graph::PosEdge* findPosEdgeByVertexPos(const Graph* graph, const sead::Vector3f& posA,
                                       const sead::Vector3f& posB) {
    for (s32 i = 0; i < graph->getEdgeCount(); i++) {
        Graph::PosEdge* edge = reinterpret_cast<Graph::PosEdge*>(graph->getEdge(i));
        Graph::PosVertex* vertex1 = edge->getVertex1();
        Graph::PosVertex* vertex2 = edge->getVertex2();
        sead::Vector3f posV1 = vertex1->getPos();
        sead::Vector3f posV2 = vertex2->getPos();
        sead::Vector3f posDiff1 = posV1 - posA;
        sead::Vector3f posDiff2 = posV2 - posB;
        if (posDiff1.length() < 10.0f && posDiff2.length() < 10.0f)
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

static bool isClosed(const sead::ConstPtrArray<Graph::Edge>& edges) {
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
            if (edges[i]->getVertex2() == edges[j]->getVertex1() &&
                edges[i]->getVertex1() == edges[j]->getVertex2())
                return false;

            if (edges[i]->getVertex1() == edges[j]->getVertex1() &&
                edges[i]->getVertex2() == edges[j]->getVertex2())
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
