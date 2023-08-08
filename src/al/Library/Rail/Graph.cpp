#include "al/graph/Graph.h"

namespace al {

float Graph::Edge::getWeight() const {
    return mWeight;
}

Graph::Graph(int vertices_size, int edges_size) {
    mVertices.allocBuffer(vertices_size, nullptr);
    mEdges.allocBuffer(edges_size, nullptr);
}

void Graph::appendVertex(int size) {
    appendVertex(new Vertex(size, mVertices.size()));
}

void Graph::appendVertex(Vertex* vertex) {
    mVertices.pushBack(vertex);
}

void Graph::removeVertex(const Vertex* vertex) {  // FIXME mismatching
    for (int i = 0; i < mVertices.size(); i++) {
        if (mVertices[i] == vertex) {
            for (int i = 0; i < vertex->getEdges().size(); i++) {
                removeEdge(vertex->getEdges()[i]);
            }
            mVertices.erase(i);
            return;
        }
    }
}

void Graph::removeEdge(const Edge* edge) {
    for (int i = 0; i < mEdges.size(); i++) {
        if (mEdges[i] == edge) {
            mEdges.erase(i);
            return;
        }
    }
}

Graph::Edge* Graph::tryFindEdge(int index_vertex1, int index_vertex2) const {
    for (int i = 0; i < mEdges.size(); i++) {
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

void Graph::appendEdge(int index_vertex1, int index_vertex2, float weight) {
    Vertex* vertex1 = mVertices[index_vertex1];
    Vertex* vertex2 = mVertices[index_vertex2];
    Edge* edge = new Edge(vertex1, vertex2, weight);

    vertex1->addEdge(edge);
    vertex2->addEdge(edge);
    mEdges.pushBack(edge);
}

bool Graph::tryAppendEdge(int index_vertex1, int index_vertex2, float weight) {
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
