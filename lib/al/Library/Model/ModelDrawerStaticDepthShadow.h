#pragma once

#include "Library/Model/ModelDrawerBase.h"

namespace al {
class ModelCtrl;

class ModelDrawerStaticDepthShadow : public ModelDrawerBase {
public:
    ModelDrawerStaticDepthShadow(const char* name);

    void createTable() override;
    void addModel(ModelCtrl*) override;
    void removeModel(ModelCtrl*) override;
    void draw() const override;
};

static_assert(sizeof(ModelDrawerStaticDepthShadow) == 0x50);

}  // namespace al
