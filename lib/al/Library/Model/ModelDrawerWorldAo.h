#pragma once

#include "Library/Model/ModelDrawerBase.h"

namespace al {

class ModelDrawerWorldAo : public ModelDrawerBase {
public:
    ModelDrawerWorldAo(const char* name);

    void createTable() override;
    void draw() const override;
    void addModel(ModelCtrl* model) override;
    void removeModel(ModelCtrl* model) override;
};

static_assert(sizeof(ModelDrawerWorldAo) == 0x50);

}  // namespace al
