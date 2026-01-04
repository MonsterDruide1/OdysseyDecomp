#pragma once

#include "Library/Model/ModelDrawerBase.h"

namespace al {
class ModelCtrl;

class ModelDrawerDepthShadow : public ModelDrawerBase {
public:
    ModelDrawerDepthShadow(const char* name, bool, bool);

    void createTable() override;
    void registerModel(ModelCtrl*) override;
    void addModel(ModelCtrl*) override;
    void updateModel(ModelCtrl*) override;
    void draw() const override;

private:
    bool _50;
};

static_assert(sizeof(ModelDrawerDepthShadow) == 0x58);

}  // namespace al
