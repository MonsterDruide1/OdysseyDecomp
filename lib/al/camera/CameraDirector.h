#pragma once

#include "CameraPoseUpdater.h"

namespace al
{
    class CameraDirector {
        public:
            void startSnapShotMode(bool);
            al::CameraPoseUpdater *getPoseUpdater(void);
            float getSceneFovyDegree(void) const;

            unsigned char padding[0x30-0x02];
            float sceneFovyDegree; // 0x30
            // 0xBC float farClipDistance
            // 0xB8 float nearClipDistance
    };

    class IUseCamera
    {
    public:
        virtual al::CameraDirector* getCameraDirector() const = 0;
    };
};