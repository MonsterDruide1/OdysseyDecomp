#pragma once

#include "Library/Model/ModelDrawerBase.h"

namespace al {
class ModelCtrl;

class ModelDrawerDepthOnly : public ModelDrawerBase {
public:
    ModelDrawerDepthOnly(const char* name, bool, bool, bool, bool, bool);

    void createTable() override;
    void draw() const override;
    void registerModel(ModelCtrl* model) override;
    void addModel(ModelCtrl* model) override;
    void removeModel(ModelCtrl* model) override;
    void updateModel(ModelCtrl* model) override;

    virtual void createTableCulling();

private:
    char filler[0x10];
};

static_assert(sizeof(ModelDrawerDepthOnly) == 0x60);

}  // namespace al
