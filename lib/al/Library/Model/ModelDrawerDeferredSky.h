#pragma once

#include "Library/Model/ModelDrawerBase.h"

namespace al {

class ModelDrawerDeferredSky : public ModelDrawerBase {
public:
    ModelDrawerDeferredSky(const char* name);

    void createTable() override;
    void draw() const override;

private:
    char filler[0x128];
};

static_assert(sizeof(ModelDrawerDeferredSky) == 0x178);

}  // namespace al
