#pragma once

#include "Library/Model/ModelDrawerBase.h"

namespace al {

class ModelDrawerBufferUpdate : public ModelDrawerBase {
public:
    ModelDrawerBufferUpdate(const char* name);

    void draw() const override;
    void createTable() override;
};

static_assert(sizeof(ModelDrawerBufferUpdate) == 0x50);

}  // namespace al
