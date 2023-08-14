#include "Library/Rail/Graph.h"

namespace al {

f32 Graph::Edge::getWeight() const {
    return mWeight;
}

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

void Graph::removeVertex(const Vertex* vertex) {  // FIXME mismatching
    for (s32 i = 0; i < mVertices.size(); i++) {
        if (mVertices[i] == vertex) {
            for (s32 i = 0; i < vertex->getEdges().size(); i++) {
                removeEdge(vertex->getEdges()[i]);
            }
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
            edge->getVertex2()->getIndex() == index_vertex2) {
            return edge;
        }
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

}  // namespace al
