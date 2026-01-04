#pragma once

#include "Library/Model/ModelDrawerBase.h"

namespace al {

class ModelDrawerDeferredFootPrint : public ModelDrawerBase {
public:
    ModelDrawerDeferredFootPrint(const char* name);

    void createTable() override;
    void draw() const override;

private:
    char filler[0x128];
};

static_assert(sizeof(ModelDrawerDeferredFootPrint) == 0x178);

}  // namespace al
