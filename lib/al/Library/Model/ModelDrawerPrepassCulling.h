#pragma once

#include "Library/Model/ModelDrawerBase.h"

namespace al {

class ModelDrawerPrepassCulling : public ModelDrawerBase {
public:
    ModelDrawerPrepassCulling(const char* name);

    void createTable() override;
    void draw() const override;
    void registerModel(ModelCtrl*) override;
    void addModel(ModelCtrl*) override;
    void updateModel(ModelCtrl*) override;
};

static_assert(sizeof(ModelDrawerPrepassCulling) == 0x50);

}  // namespace al
