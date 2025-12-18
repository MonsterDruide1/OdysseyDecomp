#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

namespace agl {
class DrawContext;
}

namespace al {
class GraphicsSystemInfo;
struct ModelDrawBufferCounter;
class ModelKeeper;
class ModelDrawDisplayList;
class ModelCtrl;
class MeshDrawerTable;

class ModelDrawerBase : public HioNode {
public:
    ModelDrawerBase(const char* name);
    virtual ~ModelDrawerBase();
    virtual void createTable() = 0;
    virtual void draw() const = 0;
    virtual void registerModel(ModelCtrl* model);
    virtual void addModel(ModelCtrl* model);
    virtual void removeModel(ModelCtrl* model);
    virtual void updateModel(ModelCtrl* model);

    void setDrawInfo(agl::DrawContext* drawCtx, const GraphicsSystemInfo* gfxInfo,
                     const ModelDrawBufferCounter* bufferCounter, const ModelKeeper* modelKeeper);
    void addModelToTable(ModelCtrl* model);
    void getDrawBufferIndex();
    void createModelDrawDisplayList(s32);
    bool isDraw() const;

private:
    const char* mName;
    agl::DrawContext* mDrawCtx;
    const ModelKeeper* mModelKeeper;
    const GraphicsSystemInfo* mGraphicsSystemInfo;
    const ModelDrawBufferCounter* mModelDrawBufferCounter;
    ModelDrawDisplayList* mDrawDisplayList;
    s32 mModelMaxNum;
    s32 mModelNum;
    ModelCtrl** mModels;
    MeshDrawerTable* mMeshDrawerTable;
};

static_assert(sizeof(ModelDrawerBase) == 0x50);

}  // namespace al
