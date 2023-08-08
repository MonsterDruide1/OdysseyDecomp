#pragma once

#include <container/seadPtrArray.h>

namespace al {

class Graph {
public:
    class Edge;

    class Vertex {
    public:
        inline Vertex(int size, int index) : mIndex(index) { mEdges.allocBuffer(size, nullptr); }

        const sead::PtrArray<Edge>& getEdges() const { return mEdges; }
        int getIndex() const { return mIndex; }
        void addEdge(Edge* edge) { mEdges.pushBack(edge); }
        void tryAddEdge(Edge* edge) {
            for (int i = 0; i < mEdges.size(); i++) {
                if (mEdges[i] == edge) {
                    return;
                }
            }
            addEdge(edge);
        }

    private:
        sead::PtrArray<Edge> mEdges;
        int mIndex;
    };
    static_assert(sizeof(Vertex) == 0x18);

    class Edge {
    public:
        Edge(Vertex* vertex1, Vertex* vertex2, float weight)
            : mVertex1(vertex1), mVertex2(vertex2), mWeight(weight) {}
        virtual float getWeight() const;

        Vertex* getVertex1() const { return mVertex1; }
        Vertex* getVertex2() const { return mVertex2; }

    private:
        Vertex* mVertex1;
        Vertex* mVertex2;
        float mWeight;
    };
    static_assert(sizeof(Edge) == 0x20);

    Graph(int vertices_size, int edges_size);
    void appendVertex(int size);
    void appendVertex(Vertex* vertex);
    void removeVertex(const Vertex* vertex);
    void removeEdge(const Edge* edge);
    Edge* tryFindEdge(int index_vertex1, int index_vertex2) const;
    void appendEdge(Edge* edge);
    bool tryAppendEdge(Edge* edge);
    void appendEdge(int index_vertex1, int index_vertex2, float weight);
    bool tryAppendEdge(int index_vertex1, int index_vertex2, float weight);

private:
    sead::PtrArray<Vertex> mVertices;
    sead::PtrArray<Edge> mEdges;
};
static_assert(sizeof(Graph) == 0x20);

}  // namespace al
