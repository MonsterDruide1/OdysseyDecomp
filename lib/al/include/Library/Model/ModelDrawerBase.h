#pragma once

#include <basis/seadTypes.h>
#include "Library/HostIO/HioNode.h"

namespace agl {
class DrawContext;
}

namespace al {
class GraphicsSystemInfo;
class ModelDrawBufferCounter;
class ModelKeeper;
class ModelDrawDisplayList;
class ModelCtrl;

class ModelDrawerBase : public HioNode {
public:
    void createTable();

    virtual void registerModel(ModelCtrl*);
    virtual void addModel(ModelCtrl*);
    virtual void removeModel(ModelCtrl*);
    virtual void updateModel(ModelCtrl*);

    void setDrawInfo(agl::DrawContext*, const GraphicsSystemInfo*, const ModelDrawBufferCounter*,
                     const ModelKeeper*);
    void addModelToTable(ModelCtrl*);
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
    s32 mCapacity;
    s32 mSize;
    ModelCtrl** mMdlCtrls;
    void* field_48;
};
}  // namespace al
