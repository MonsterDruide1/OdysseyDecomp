#pragma once

#include "Library/Model/ModelDrawerBase.h"

namespace al {
class ModelCtrl;

class ModelDrawerChromakey : public ModelDrawerBase {
public:
    ModelDrawerChromakey(const char* name, bool, bool, bool, bool);

    void createTable() override;
    void registerModel(ModelCtrl* model) override;
    void draw() const override;

private:
    bool _50;
    bool _51;
    bool _52;
    bool _53;
};

static_assert(sizeof(ModelDrawerChromakey) == 0x58);

}  // namespace al
