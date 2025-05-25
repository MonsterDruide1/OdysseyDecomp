#pragma once

#include "Library/Nerve/NerveExecutor.h"

namespace al {
class GraphicsQualityInfo;

class GraphicsQualityController : public NerveExecutor {
public:
    // missing

    GraphicsQualityInfo* getGraphicsQualityInfo() const { return mGraphicsQualityInfo; }

    bool isChangedGraphicsQualityMode() const { return mIsChangedGraphicsQualityMode; }

private:
    void* _0[1];
    GraphicsQualityInfo* mGraphicsQualityInfo;
    void* _20[17];
    bool mIsChangedGraphicsQualityMode;
    void* _b0[11];
};

static_assert(sizeof(GraphicsQualityController) == 0x108);

}  // namespace al
