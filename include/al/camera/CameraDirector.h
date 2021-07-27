#pragma once

namespace al
{
    class CameraDirector;

    class IUseCamera
    {
    public:
        virtual al::CameraDirector* getCameraDirector() const = 0;
    };
};