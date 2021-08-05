#pragma once

namespace sead
{
    namespace hostio
    {
        // all of these are unused in release builds, but used in develop / debug builds
        class LifeCheckable { };
        class PropertyEventListener : public LifeCheckable { };
        class NodeEventListener : public PropertyEventListener { };
        class Reflexible : public NodeEventListener { };
    };
};