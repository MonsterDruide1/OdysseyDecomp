#pragma once

namespace al
{
    class Graph
    {
        class Edge
        {
        public:
            virtual float getWeight() const;

            unsigned long _8;
            unsigned long _10;
            float mWeight; // _18
        };

        class PosEdge : public Edge
        {
        public:
            virtual float getWeight() const;
        };

        class ElectricWireEdge : public PosEdge
        {
        public:
            virtual float getWeight() const;
        };
    };
};