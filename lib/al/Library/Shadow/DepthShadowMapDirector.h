#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class DepthShadowMapCtrl;
class ModelKeeper;

class DepthShadowMapDirector : public HioNode {
public:
    virtual ~DepthShadowMapDirector();

    void createDepthShadowMap(const DepthShadowMapCtrl*, const ModelKeeper*, const char*, s32, s32,
                              s32);

    void declareUseDepthShadowMap(s32 num) { _68 += num; }

private:
    void* _8[0x60 / 8];
    s32 _68;
    s32 _6c;
    void* _70[0x58 / 8];
};

static_assert(sizeof(DepthShadowMapDirector) == 0xc8);

}  // namespace al
